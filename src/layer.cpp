#include "layer.h"
void TreeLayer::on_attach() {
  Layer::on_attach();
}
void TreeLayer::on_update(float dt) {
  gl::set_clear_color({0.3, 0.3, 0.3, 1.0});
  gl::clear();

  mesh_.bind();
  shader->use();

  gl::draw_elements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}
void TreeLayer::on_event(const WindowEvent& event) {
  Layer::on_event(event);
}
void TreeLayer::on_gui() {
  Layer::on_gui();
}
