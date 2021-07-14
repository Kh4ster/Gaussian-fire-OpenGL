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
class Lights
{
  public:
    void add_light(const Light& light)
    {
        origins_.push_back(light.origin_);
        strengths_.push_back(light.strength_);
    }

    size_t size() { return origins_.size(); }

    glm::vec3* const origins_get() { return origins_.data(); }
    float* strengths_get() { return strengths_.data(); }

    Lights() = default;

  private:
    std::vector<glm::vec3> origins_;
    std::vector<float> strengths_;
};

static Lights lights;

} // namespace scene