#pragma once

#include <glm/glm.hpp>

namespace scene
{
struct Light final
{
  public:
    Light(const glm::vec3 origin, const float strength)
        : origin_(origin)
        , strength_(strength)
    {
    }

    Light() = default;

    glm::vec3 origin_;
    float strength_;
};

static Light main_light;
} // namespace scene