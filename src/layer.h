#pragma once

#include "mesh.h"
#include "tree.h"

#include <gl/all.hpp>
#include <vgl/layer.h>
#include <vgl/shader.h>

class TreeLayer : public Layer {
 public:
  TreeLayer();
  void on_attach() override;
  void on_update(float dt) override;
  void on_event(const WindowEvent& event) override;
  void on_gui() override;

 private:
  std::shared_ptr<gl::program> shader;
  Tree tree_;
  glm::vec3 camera_position_{0.0f, 4.0f, -3.0f};
  glm::mat4 projection_{glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.01f, 1000.0f)};
  bool show_bound_box_{true};
};
