#include "node.h"

void Node::grow(float feed) {
  radius_ = std::sqrt(area_ / static_cast<float>(M_PI));

  if (is_leaf_) {
    length_ += std::cbrt(feed);
    feed -= std::cbrt(feed) * area_;
    area_ += feed / length_;

    if (length_ > params_->split_size * std::exp(-params_->split_decay * static_cast<float>(depth_))) {
      split();
    }
    return;
  }

  float pass = params_->pass_ratio;

  if (params_->conserve_area) {
    pass = (left_->area_ + right_->area_) / (left_->area_ + right_->area_ + area_);
  }

  area_ += pass * feed / length_;
  feed *= (1.0f - pass);

  if (feed < 1e-5) {
    return;
  }

  left_->grow(feed * params_->ratio);
  right_->grow(feed * (1.0f - params_->ratio));
}
void Node::split() {
  float flip = rand() % 2 ? -1.0f : 1.0f;

  const glm::vec3 D = calculate_leaf_density_dir(params_->local_depth);
  const glm::vec3 N = glm::normalize(glm::cross(dir_, D));
  const glm::vec3 M = -N;

  left_ = std::make_unique<Node>(this, glm::mix(flip * params_->spread * N, dir_, params_->ratio));
  right_ = std::make_unique<Node>(this, glm::mix(flip * params_->spread * M, dir_, params_->ratio));
  is_leaf_ = false;
}
glm::vec3 Node::calculate_leaf_density_dir(int depth) const {
  const auto r = glm::vec3(rand() % 100, rand() % 100, rand() % 100) / glm::vec3(100) - glm::vec3(0.5f);

  if (depth == 0) return r;

  const Node* C = this;
  glm::vec3 rel(0.0f);
  while (C->depth_ > 0 && depth-- >= 0) {
    rel += C->length_ * C->dir_;
    C = C->parent_;
  }

  std::function<glm::vec3(const Node*)> leafaverage = [&](const Node* b) -> glm::vec3 {
    if (b->is_leaf_) return b->length_ * b->dir_;
    return b->length_ * b->dir_ + params_->ratio * leafaverage(b->left_.get()) + (1.0f - params_->ratio) * leafaverage(b->right_.get());
  };

  return params_->directedness * glm::normalize(leafaverage(C) - rel) + (1.0f - params_->directedness) * r;
}
