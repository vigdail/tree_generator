#include "layer.h"

#include <GLFW/glfw3.h>
#include <gl/auxiliary/glm_uniforms.hpp>
#include <imgui.h>

#include <filesystem>

int i = 0;

namespace Color {
constexpr auto Tree = glm::vec3(1.0f, 0.0f, 0.0f);
constexpr auto BoundBox = glm::vec3(0.0f, 1.0f, 0.0f);
}// namespace Color

void load_shader_include(std::string_view name, const std::filesystem::path& filename) {
  std::ifstream stream(filename);
  const std::string content{std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
  glNamedStringARB(GL_SHADER_INCLUDE_ARB, static_cast<GLint>(name.size()), name.data(), static_cast<GLint>(content.size()), content.c_str());
}

TreeLayer::TreeLayer() {
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

  const float radius = 7.0f;
  const auto time = static_cast<float>(glfwGetTime());
  const float x = radius * std::sin(time);
  const float z = radius * std::cos(time);
  camera_position_.x = x;
  camera_position_.y = 5.0f;
  camera_position_.z = z;
  const glm::mat4 view = glm::lookAt(camera_position_, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  const glm::mat4 view_proj = projection_ * view;
  shader->set_uniform(shader->uniform_location("view_proj"), view_proj);
  shader->set_uniform(shader->uniform_location("color"), Color::Tree);

  if (i++ < 100) {
    tree_.grow(0.5f);
    tree_.update_mesh();
    tree_.bound_box.update_mesh();
  }

  tree_.mesh.bind();
  gl::set_polygon_mode(GL_FILL);
  gl::set_polygon_face_culling_enabled(true);
  auto count = static_cast<int32_t>(tree_.mesh.get_index_count());
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

void TreeLayer::on_gui() {
  ImGui::Checkbox("Show bound box", &show_bound_box_);
  rttr::type class_type = rttr::type::get(tree_);
  for (const auto& prop: class_type.get_properties()) {
    rttr::variant value = prop.get_value(tree_);
    if (value.is_type<int>()) {
      auto v = value.get_value<int>();
      if (ImGui::InputInt(prop.get_name().to_string().c_str(), &v)) {
        prop.set_value(tree_, v);
      }
    }
    if (value.is_type<float>()) {
      auto v = value.get_value<float>();
      if (ImGui::InputFloat(prop.get_name().to_string().c_str(), &v)) {
        prop.set_value(tree_, v);
      }
    }
  }
  if (ImGui::Button("Generate")) {
    tree_.generate();
    i = 0;
  }
}
