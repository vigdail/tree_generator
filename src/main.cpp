#include "layer.h"

#include <vgl/application.h>

int main() {
  Application app("Tree", 1280, 720);
  app.push_layer(std::make_unique<TreeLayer>());
  app.run();
  return 0;
}
