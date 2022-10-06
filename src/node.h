#pragma once
#include "mesh.h"

#include <glm/gtx/quaternion.hpp>
#include <rttr/registration.h>

#include <cmath>

#include <iomanip>
#include <iostream>
#include <memory>

struct TreeParameters {
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
  RTTR_ENABLE()
};

RTTR_REGISTRATION {
  using namespace rttr;
  registration::class_<TreeParameters>("tree_parameters_class")
      .property("ring_size", &TreeParameters::ring_size)
      .property("taper", &TreeParameters::taper)
      .property("ratio", &TreeParameters::ratio)
      .property("spread", &TreeParameters::spread)
      .property("split_size", &TreeParameters::split_size)
      .property("split_decay", &TreeParameters::split_decay)
      .property("conserve_area", &TreeParameters::conserve_area)
      .property("pass_ratio", &TreeParameters::pass_ratio)
      .property("local_depth", &TreeParameters::local_depth)
      .property("directedness", &TreeParameters::directedness);
}

class Node {
 public:
  explicit Node(TreeParameters* params) : params_{params} {
    srand(time(nullptr));// TODO: use proper RNG
  }

  Node(Node* parent, const glm::vec3& dir) : Node(parent->params_) {
    dir_ = dir;
    parent_ = parent;
    depth_ = parent->depth_ + 1;
  }

  [[nodiscard]] float get_area() const {
    return area_;
  }

  [[nodiscard]] int get_depth() const {
    return depth_;
  }

  [[nodiscard]] float get_length() const {
    return length_;
  }

  [[nodiscard]] bool is_leaf() const {
    return is_leaf_;
  }

  [[nodiscard]] const Node* left() const {
    return left_.get();
  }

  [[nodiscard]] const Node* right() const {
    return right_.get();
  }

  [[nodiscard]] const glm::vec3& get_dir() const {
    return dir_;
  }

  [[nodiscard]] float get_radius() const {
    return radius_;
  }

  void grow(float feed);

 private:
  void split();
  [[nodiscard]] glm::vec3 calculate_leaf_density_dir(int depth) const;

 private:
  glm::vec3 dir_{0.0f, 1.0f, 0.0f};
  bool is_leaf_{true};
  TreeParameters* params_;
  int depth_{0};
  float length_{0.0};
  float radius_{0.0};
  float area_{0.1};
  //  float ratio_;
  //  float spread_;
  //  float split_size_;
  std::unique_ptr<Node> left_{};
  std::unique_ptr<Node> right_{};
  Node* parent_{};
};