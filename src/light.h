#pragma once

#include <glm/glm.hpp>
#include <rttr/type>
#include <rttr/registration>

class Light {
 public:
  glm::vec3 direction{1.0f, 1.0f, 1.0f};
  float intensity{1.0f};

  RTTR_ENABLE()
};

