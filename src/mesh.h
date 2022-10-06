#pragma once

#include <gl/all.hpp>
#include <glm/glm.hpp>

#include <vector>

struct Vertex {
  glm::vec3 pos;
};

class Mesh {
 public:
  Mesh() {
    std::vector<Vertex> vertices{
        {{0.0f, 0.5f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}},
    };

    std::vector<uint32_t> indices{0, 1, 2};

    vertex_buffer_.set_data(static_cast<ptrdiff_t>(sizeof(Vertex) * vertices.size()), vertices.data(), GL_STATIC_DRAW);
    index_buffer_.set_data(static_cast<ptrdiff_t>(sizeof(uint32_t) * indices.size()), indices.data(), GL_STATIC_DRAW);

    vertex_array_.bind();
    vertex_array_.set_vertex_buffer(0, vertex_buffer_, 0, sizeof(Vertex));
    vertex_array_.set_element_buffer(index_buffer_);
    vertex_array_.set_attribute_enabled(0, true);
    vertex_array_.set_attribute_format(0, 3, GL_FLOAT, false, 0);
    gl::vertex_array::unbind();

    index_count_ = index_buffer_.size();
  }

  void update(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    const size_t vertices_size = sizeof(Vertex) * vertices.size();
    if (vertex_buffer_.size() < vertices_size) {
      vertex_buffer_.set_data(static_cast<ptrdiff_t>(vertices_size), vertices.data(), GL_DYNAMIC_DRAW);
    } else {
      vertex_buffer_.set_sub_data(0, static_cast<ptrdiff_t>(vertices_size), vertices.data());
    }

    const size_t indices_size = sizeof(uint32_t) * indices.size();
    if (index_buffer_.size() < indices_size) {
      index_buffer_.set_data(static_cast<ptrdiff_t>(indices_size), indices.data(), GL_DYNAMIC_DRAW);
    } else {
      index_buffer_.set_sub_data(0, static_cast<ptrdiff_t>(indices_size), indices.data());
    }

    index_count_ = indices.size();
  }

  void bind() const {
    vertex_array_.bind();
  }

  [[nodiscard]] size_t get_index_count() const {
    return index_count_;
  }

 private:
  size_t index_count_{};
  gl::vertex_array vertex_array_{};
  gl::buffer vertex_buffer_{};
  gl::buffer index_buffer_{};
};
