#pragma once
#include "mesh.h"
#include <cmath>
#include <glm/gtx/quaternion.hpp>
#include <iomanip>
#include <iostream>
#include <memory>
class Node {
 public:
  Node(float ratio, float spread, float split_size) : ratio_{ratio}, spread_{spread}, split_size_{split_size} {
    srand(time(nullptr)); // TODO: use proper RNG
  }

  Node(Node* parent, const glm::vec3& dir) : Node(parent->ratio_, parent->spread_, parent->split_size_) {
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
  int depth_{0};
  float length_{0.0};
  float radius_{0.0};
  float area_{0.1};
  float ratio_;
  float spread_;
  float split_size_;
  std::unique_ptr<Node> left_{};
  std::unique_ptr<Node> right_{};
  Node* parent_{};
};