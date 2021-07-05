#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "program_propreties.hh"

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
        view = glm::lookAt(origin_, origin_ + target_, up_);
        return view;
    };

    void update_camera(const Camera& camera)
    {
        GLint loc = glGetUniformLocation(program_id, "model_view_matrix");
        if (loc != -1)
            glUniformMatrix4fv(loc,
                               1,
                               GL_FALSE,
                               glm::value_ptr(camera.get_model_view_matrix()));

        loc = glGetUniformLocation(program_id, "projection_matrix");
        if (loc != -1)
            glUniformMatrix4fv(loc,
                               1,
                               GL_FALSE,
                               glm::value_ptr(camera.get_projection_matrix()));
    }

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

    const float base_speed = 6.f;
};

} // namespace scene

// Camera
scene::Camera
    camera({0.f, 0.f, -10.f}, // origin = camera axis
           {0.f, 0.f, 1.f},   // target = The *point* we look at in the scene
           {0.f, 1.f, 0.f},   // up vector
           0.5f,              // z_min
           100.f,             // z_max
           90.f,              // alpha
           window_width,      // width
           window_height);    // height