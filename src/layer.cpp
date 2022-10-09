#include "layer.h"

#include <GLFW/glfw3.h>
#include <gl/auxiliary/glm_uniforms.hpp>
#include <imgui.h>

#include <filesystem>

int grow_iterations = 0;
const int max_grow_iterations = 1000;
float grow_rate = 1.0f;
float camera_distance = 25.0f;
float camera_height = 10.0f;
float camera_target_height = 8.0f;

namespace Color {
constexpr auto Tree = glm::vec3(1.0f, 0.0f, 0.0f);
constexpr auto Floor = glm::vec3(0.5f, 0.5f, 0.5f);
constexpr auto BoundBox = glm::vec3(0.0f, 1.0f, 0.0f);
}// namespace Color

class MeshFactory {
 public:
  static Mesh create_plane(float size = 1.0f) {
    const std::vector<Vertex> vertices{
        {glm::vec3(0.5f, 0.0f, 0.5f) * size},
        {glm::vec3(-0.5f, 0.0f, 0.5f) * size},
        {glm::vec3(-0.5f, 0.0f, -0.5f) * size},
        {glm::vec3(0.5f, 0.0f, -0.5f) * size},
    };
    const std::vector<uint32_t> indices{0, 2, 1, 2, 0, 3};
    Mesh mesh;
    mesh.update(vertices, indices);

    return mesh;
  }
};

void load_shader_include(std::string_view name, const std::filesystem::path& filename) {
  std::ifstream stream(filename);
  const std::string content{std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
  glNamedStringARB(GL_SHADER_INCLUDE_ARB, static_cast<GLint>(name.size()), name.data(), static_cast<GLint>(content.size()), content.c_str());
}

TreeLayer::TreeLayer() : floor_{MeshFactory::create_plane(10.0f)} {
  load_shader_include("/scene.glsl", "../assets/scene.glsl");
  shader = load_shader("../assets/solid.vert", "../assets/solid.frag");
}

void TreeLayer::on_attach() {
  gl::set_depth_test_enabled(true);
  tree_.mesh.bind();

  shader->use();
}

void TreeLayer::on_update(float dt) {
  gl::set_clear_color({0.3, 0.3, 0.3, 1.0});
  gl::clear();

  const float radius = camera_distance;
  const auto time = static_cast<float>(glfwGetTime());
  const float x = radius * std::sin(time);
  const float z = radius * std::cos(time);
  camera_position_.x = x;
  camera_position_.y = camera_height;
  camera_position_.z = z;
  const glm::mat4 view = glm::lookAt(camera_position_, glm::vec3(0.0f, camera_target_height, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  const glm::mat4 view_proj = projection_ * view;
  shader->set_uniform(shader->uniform_location("view_proj"), view_proj);
  shader->set_uniform(shader->uniform_location("color"), Color::Tree);

  if (grow_iterations++ < max_grow_iterations) {
    tree_.grow(grow_rate);
    tree_.update_mesh();
    tree_.bound_box.update_mesh();
  }

  gl::set_polygon_mode(GL_FILL);
  tree_.mesh.bind();
  gl::set_polygon_face_culling_enabled(true);
  auto count = static_cast<int32_t>(tree_.mesh.get_index_count());
  gl::draw_elements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

  floor_.bind();
  shader->set_uniform(shader->uniform_location("color"), Color::Floor);
  count = static_cast<int32_t>(floor_.get_index_count());
  gl::draw_elements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

  if (show_bound_box_) {
    gl::set_polygon_mode(GL_LINE);
    gl::set_polygon_face_culling_enabled(false);
    shader->set_uniform(shader->uniform_location("color"), Color::BoundBox);
    count = static_cast<int32_t>(tree_.bound_box.get_mesh().get_index_count());
    tree_.bound_box.get_mesh().bind();
    gl::draw_elements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
  }
}

void TreeLayer::on_event(const WindowEvent& event) {
  Layer::on_event(event);
}

template<typename T>
T get_metadata_or_default(const rttr::property& prop, const rttr::variant& key, const T& default_value) {
  const auto& meta = prop.get_metadata(key);
  if (meta.is_valid()) {
    return meta.template get_value<T>();
  }
  return default_value;
}

void TreeLayer::on_gui() {
  ImGui::Begin("Parameters");

  ImGui::Text("Camera");
  ImGui::DragFloat("Distance", &camera_distance, 0.1f, 1.0f, 30.0f);
  ImGui::DragFloat("Height", &camera_height, 0.1f, 0.0f, 30.0f);
  ImGui::DragFloat("Target Height", &camera_target_height, 0.1f, 0.0f, 30.0f);

  ImGui::Spacing();

  ImGui::Text("Tree: %d triangles", tree_.mesh.get_index_count() / 3);
  ImGui::Checkbox("Show bound box", &show_bound_box_);
  ImGui::DragFloat("Grow rate", &grow_rate, 0.01f, 0.0f, 5.0f);
  rttr::type class_type = rttr::type::get(tree_.parameters);
  for (const auto& prop: class_type.get_properties()) {
    rttr::variant value = prop.get_value(tree_.parameters);
    if (value.is_type<int>()) {
      auto v = value.get_value<int>();
      int min = get_metadata_or_default(prop, MetaData::MinValue, 0);
      int max = get_metadata_or_default(prop, MetaData::MaxValue, 0);
      assert(min <= max);
      if (min != max) {
        if (ImGui::SliderInt(prop.get_name().to_string().c_str(), &v, min, max)) {
          prop.set_value(tree_.parameters, v);
        }
      } else {
        if (ImGui::DragInt(prop.get_name().to_string().c_str(), &v, 1, min, max)) {
          prop.set_value(tree_.parameters, v);
        }
      }
    }
    if (value.is_type<float>()) {
      auto v = value.get_value<float>();
      float min = get_metadata_or_default(prop, MetaData::MinValue, 0.0f);
      float max = get_metadata_or_default(prop, MetaData::MaxValue, 0.0f);
      float step = get_metadata_or_default(prop, MetaData::Step, 1.0f);
      if (ImGui::DragFloat(prop.get_name().to_string().c_str(), &v, step, min, max)) {
        prop.set_value(tree_.parameters, v);
      }
    }
    if (value.is_type<bool>()) {
      auto v = value.get_value<bool>();
      if (ImGui::Checkbox(prop.get_name().to_string().c_str(), &v)) {
        prop.set_value(tree_.parameters, v);
      }
    }
  }
  if (ImGui::Button("Generate")) {
    tree_.generate();
    grow_iterations = 0;
  }
  ImGui::End();
}
