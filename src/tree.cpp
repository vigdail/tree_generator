#include "tree.h"

#include <limits>

Tree::Tree() : root{new Node(&parameters)} {
}

void Tree::update_mesh() {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;

  glm::vec2 tree_scale{1.0f, 0.1f};
  std::function<void(const Node* node, const glm::vec3& p)> add_node = [&](const Node* node, const glm::vec3& p) {
    glm::vec3 start = p;
    glm::vec3 end = start + glm::vec3(node->get_length() * tree_scale.x) * node->get_dir();

    const auto d = glm::normalize(node->get_dir() + glm::vec3(1.0f));
    auto n = glm::vec4(glm::normalize(glm::cross(node->get_dir(), d)), 1.0f);

    const glm::mat4 r = glm::rotate(glm::mat4(1.0f), float(M_PI / parameters.ring_size), node->get_dir());

    const size_t _b = vertices.size();

    for (int i = 0; i < parameters.ring_size; i++) {
      indices.push_back(_b + i * 2 + 0);
      indices.push_back(_b + (i * 2 + 2) % (2 * parameters.ring_size));
      indices.push_back(_b + i * 2 + 1);

      indices.push_back(_b + (i * 2 + 2) % (2 * parameters.ring_size));
      indices.push_back(_b + (i * 2 + 3) % (2 * parameters.ring_size));
      indices.push_back(_b + i * 2 + 1);
    }

    for (int i = 0; i < parameters.ring_size; i++) {
      vertices.push_back({start + node->get_radius() * tree_scale[1] * glm::vec3(n)});
      n = r * n;

      vertices.push_back({end + parameters.taper * node->get_radius() * tree_scale[1] * glm::vec3(n)});
      n = r * n;
    }

    if (node->is_leaf()) return;

    add_node(node->left(), end);
    add_node(node->right(), end);
  };

  add_node(root.get(), {});

  for (const auto& vertex: vertices) {
    bound_box.min.x = std::min(bound_box.min.x, vertex.pos.x);
    bound_box.min.y = std::min(bound_box.min.y, vertex.pos.y);
    bound_box.min.z = std::min(bound_box.min.z, vertex.pos.z);

    bound_box.max.x = std::max(bound_box.max.x, vertex.pos.x);
    bound_box.max.y = std::max(bound_box.max.y, vertex.pos.y);
    bound_box.max.z = std::max(bound_box.max.z, vertex.pos.z);
  }

  mesh.update(vertices, indices);
}
void Tree::grow(float feed) {
  root->grow(feed);
}
void Tree::generate() {
  root = std::make_unique<Node>(&parameters);
  bound_box.min = glm::vec3(std::numeric_limits<float>::max());
  bound_box.max = glm::vec3(std::numeric_limits<float>::min());
}
