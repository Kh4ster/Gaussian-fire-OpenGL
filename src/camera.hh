#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scene
{
struct Camera final
{
    Camera(const glm::vec3& origin,
           const glm::vec3& target,
           const glm::vec3& up,
           const float z_min,
           const float z_max,
           const float alpha,
           const int width,
           const int height)
        : origin_(origin)
        , target_(target)
        , up_(up)
        , z_min_(z_min)
        , z_max_(z_max)
        , alpha_(alpha)
        , width_(width)
        , height_(height)
    {
    }

    Camera(const Camera&) = default;
    Camera& operator=(const Camera&) = default;

    ~Camera() = default;

    glm::mat4 get_projection_matrix() const
    {
        return glm::perspectiveFov(alpha_, width_, height_, z_min_, z_max_);
    }

    glm::mat4 get_model_view_matrix() const
    {
        glm::mat4 view;
        view = glm::lookAt(origin_, target_, up_);
        return view;
    };

    // Origin of the camera `C` in 3D world
    glm::vec3 origin_;

    // Up vector in 3D world
    glm::vec3 up_;
    // Target point of the camera in 3D world
    glm::vec3 target_;

    // Focal distances
    float z_min_;
    float z_max_;

    // Field of view in degree
    float alpha_;

    // Image dimensions
    float width_;
    float height_;
};

} // namespace scene