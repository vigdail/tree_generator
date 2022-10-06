#pragma once

#include "bound_box.h"
#include "mesh.h"
#include "node.h"

#include <glm/gtx/quaternion.hpp>
#include <rttr/registration>
#include <rttr/type>

#include <cmath>

#include <iomanip>
#include <iostream>
#include <memory>

class Tree {
 public:
  Tree();
  void grow(float feed);
  void update_mesh();
  void generate();

 public:
  Mesh mesh;
  BoundBox bound_box;
  TreeParameters parameters{};

 private:
  std::unique_ptr<Node> root;
};
