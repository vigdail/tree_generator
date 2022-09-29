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

    vertex_buffer_.set_data(sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    index_buffer_.set_data(sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

    vertex_array_.bind();
    vertex_array_.set_vertex_buffer(0, vertex_buffer_, 0, sizeof(Vertex));
    vertex_array_.set_element_buffer(index_buffer_);
    vertex_array_.set_attribute_enabled(0, true);
    vertex_array_.set_attribute_format(0, 3, GL_FLOAT, false, 0);
    gl::vertex_array::unbind();
  }

  void bind() const {
    vertex_array_.bind();
  }

 private:
  gl::vertex_array vertex_array_{};
  gl::buffer vertex_buffer_{};
  gl::buffer index_buffer_{};
};
