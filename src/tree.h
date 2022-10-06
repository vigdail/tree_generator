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

  int ring_size{12};
  float taper{0.6f};
  float ratio{0.3f};
  float spread{0.5f};
  float split_size{1.0f};
  float split_decay{0.3f};
  bool conserve_area{true};
  float pass_ratio{0.5f};
  int local_depth{2};
  float directedness{0.3f};

 private:
  std::unique_ptr<Node> root;

  RTTR_ENABLE()
};

RTTR_REGISTRATION {
  using namespace rttr;
  registration::class_<Tree>("tree_class")
      .property("ring_size", &Tree::ring_size)
      .property("taper", &Tree::taper)
      .property("ratio", &Tree::ratio)
      .property("spread", &Tree::spread)
      .property("split_size", &Tree::split_size)
      .property("split_decay", &Tree::split_decay)
      .property("conserve_area", &Tree::conserve_area)
      .property("pass_ratio", &Tree::pass_ratio)
      .property("local_depth", &Tree::local_depth)
      .property("directedness", &Tree::directedness);
};
