#pragma once

#include <cmath>

#include <memory>
#include <iostream>
#include <iomanip>

class Node {
 public:
  Node(float ratio, float spread, float split_size) : ratio_{ratio}, spread_{spread}, split_size_{split_size} {}
  explicit Node(Node* parent) : Node(parent->ratio_, parent->spread_, parent->split_size_) {
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
  void grow(float feed) {
    radius_ = std::sqrt(area_ / static_cast<float>(M_PI));

    if (is_leaf_) {
      float split_decay = 10.0f;
      length_ += std::cbrt(feed);
      feed -= std::cbrt(feed) * area_;
      area_ += feed / length_;

      if (length_ > split_size_ * std::exp(-split_decay * static_cast<float>(depth_))) {
        split();
      }
      return;
    }

    float pass_ratio = 0.5f;
    float pass = pass_ratio;
    bool conserve_area = false;

    if (conserve_area) {
      pass = (left_->area_ + right_->area_) / (left_->area_ + right_->area_ + area_);
    }

    area_ += pass * feed / length_;
    feed *= (1.0f - pass);

    if (feed < 1e-5) {
      return;
    }

    left_->grow(feed * ratio_);
    right_->grow(feed * (1.0f - ratio_));
  }

 private:
  void split() {
    is_leaf_ = false;
    left_ = std::make_unique<Node>(this);
    right_ = std::make_unique<Node>(this);
  }

 private:
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

class Tree {
 public:
  Tree() : root {0.1, 0.5, 1.0f} {
    for (int i = 0; i < 6; ++i) {
      root.grow(0.01f);
    }
    print(&root);
  }

  void print(const Node* node) {
    std::cout << std::setw(node->get_depth()*2) << "" << "length: " << node->get_length() << ", area: " << node->get_area() << '\n';
    if (!node->is_leaf()) {
      print(node->left());
      print(node->right());
    }

  }

 private:
  Node root;
};
