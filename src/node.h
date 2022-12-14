#pragma once
#include "mesh.h"
#include "reflection.h"

#include <glm/gtx/quaternion.hpp>
#include <rttr/registration.h>

#include <cmath>

#include <iomanip>
#include <iostream>
#include <memory>

struct TreeParameters {
  int ring_size{12};
  float taper{0.6f};
  float ratio{0.8f};
  float spread{1.2f};
  float split_size{2.0f};
  float split_decay{0.0f};
  bool conserve_area{true};
  float pass_ratio{0.5f};
  int local_depth{1};
  float directedness{0.9f};
  RTTR_ENABLE()
};

RTTR_REGISTRATION {
  using namespace rttr;
  registration::class_<TreeParameters>("tree_parameters_class")
      .property("ring_size", &TreeParameters::ring_size)(
          metadata(MetaData::MinValue, 3),
          metadata(MetaData::MaxValue, 24))
      .property("taper", &TreeParameters::taper)(
          metadata(MetaData::MinValue, 0.1f),
          metadata(MetaData::MaxValue, 1.0f),
          metadata(MetaData::Step, 0.01f))
      .property("ratio", &TreeParameters::ratio)(
          metadata(MetaData::MinValue, 0.01f),
          metadata(MetaData::MaxValue, 0.99f),
          metadata(MetaData::Step, 0.01f))
      .property("spread", &TreeParameters::spread)(
          metadata(MetaData::MinValue, 0.0f),
          metadata(MetaData::MaxValue, 5.0f),
          metadata(MetaData::Step, 0.01f))
      .property("split_size", &TreeParameters::split_size)(
          metadata(MetaData::MinValue, 0.1f),
          metadata(MetaData::MaxValue, 5.0f),
          metadata(MetaData::Step, 0.1f))
      .property("split_decay", &TreeParameters::split_decay)(
          metadata(MetaData::MinValue, 0.0f),
          metadata(MetaData::MaxValue, 1.0f),
          metadata(MetaData::Step, 0.001f))
      .property("conserve_area", &TreeParameters::conserve_area)
      .property("pass_ratio", &TreeParameters::pass_ratio)
      .property("local_depth", &TreeParameters::local_depth)(
          metadata(MetaData::MinValue, 0),
          metadata(MetaData::MaxValue, 15),
          metadata(MetaData::Step, 1))
      .property("directedness", &TreeParameters::directedness)(
          metadata(MetaData::MinValue, 0.0f),
          metadata(MetaData::MaxValue, 1.0f),
          metadata(MetaData::Step, 0.01f));
}

class Node {
 public:
  explicit Node(TreeParameters* params) : params_{params} {}

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
  std::unique_ptr<Node> left_{};
  std::unique_ptr<Node> right_{};
  Node* parent_{};
};