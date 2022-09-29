#pragma once

#include "mesh.h"
#include "tree.h"

#include <gl/all.hpp>
#include <vgl/layer.h>
#include <vgl/shader.h>

class TreeLayer : public Layer {
 public:
  void on_attach() override;
  void on_update(float dt) override;
  void on_event(const WindowEvent& event) override;
  void on_gui() override;

 private:
  std::shared_ptr<gl::program> shader{load_shader("../assets/solid.vert", "../assets/solid.frag")};
  Mesh mesh_;
  Tree tree;
};
