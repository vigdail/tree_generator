#pragma once

#include "mesh.h"

class BoundBox {
 public:
  BoundBox() : vertices_(8) {}
  [[nodiscard]] const Mesh& get_mesh() const;
  void update_mesh();

 public:
  glm::vec3 min{};
  glm::vec3 max{};

 private:
  Mesh mesh_{};
  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_{
      0, 1, 2, 2, 3, 0,// BOTTOM
      4, 5, 6, 6, 7, 4,// TOP
      0, 3, 4, 4, 7, 3,// LEFT
      1, 2, 5, 5, 6, 2,// RIGHT
      0, 1, 4, 4, 5, 1, // BACK
      3, 2, 7, 7, 6, 2, // BACK
  };

  // 0-----1
  // |     |
  // |     |
  // 3-----2
  // 4-----5
  // |     |
  // |     |
  // 7-----6
};
