#include "light.h"
#include "reflection.h"

RTTR_REGISTRATION {
  using namespace rttr;
  registration::class_<Light>("light")
      .property("intensity", &Light::intensity)(
          metadata(MetaData::MinValue, 0.0f),
          metadata(MetaData::MaxValue, 5.0f),
          metadata(MetaData::Step, 0.01f))
      .property("direction", &Light::direction)(
          metadata(MetaData::MinValue, -1.0f),
          metadata(MetaData::MaxValue, 1.0f));
};