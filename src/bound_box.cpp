#include "bound_box.h"
const Mesh& BoundBox::get_mesh() const {
  return mesh_;
}

void BoundBox::update_mesh() {
  vertices_[0].pos = min;
  vertices_[1].pos = glm::vec3(max.x, min.y, min.z);
  vertices_[2].pos = glm::vec3(max.x, min.y, max.z);
  vertices_[3].pos = glm::vec3(min.x, min.y, max.z);

  vertices_[4].pos = glm::vec3(min.x, max.y, min.z);
  vertices_[5].pos = glm::vec3(max.x, max.y, min.z);
  vertices_[6].pos = max;
  vertices_[7].pos = glm::vec3(min.x, max.y, max.z);

  for (int i = 0; i < 4; ++i) {
    assert(vertices_[i].pos.x == vertices_[i + 4].pos.x);
    assert(vertices_[i].pos.z == vertices_[i + 4].pos.z);
  }

  mesh_.update(vertices_, indices_);
}
