#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "program_propreties.hh"
#include "camera.hh"

struct MouseHandler
{
    static bool init_mouse;
    static double last_x;
    static double last_y;
    static double yaw;
    static double pitch;

    static void handle_mouse(int xpos, int ypos)
    {
        if (init_mouse)
        {
            last_x = static_cast<double>(xpos);
            last_y = static_cast<double>(ypos);
            init_mouse = false;
        }

        float x_offset = xpos - last_x;
        float y_offset = last_y - ypos;
        last_x = xpos;
        last_y = ypos;

        constexpr float sensitivity = 0.1f;
        x_offset *= sensitivity;
        y_offset *= sensitivity;

        yaw += x_offset;
        pitch += y_offset;

        pitch = std::min(pitch, 89.0);
        pitch = std::max(pitch, -89.0);

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera.target_ = glm::normalize(direction);

        if (fabs(last_x - window_center_x) > 250.0 ||
            fabs(last_y - window_center_y) > 250.0)
        {
            glutWarpPointer(window_center_x, window_center_y);
            last_x = window_center_x;
            last_y = window_center_y;
        }

        camera.update_camera();
        glutPostRedisplay();
    }
};

bool MouseHandler::init_mouse = true;
double MouseHandler::last_x = window_center_x;
double MouseHandler::last_y = window_center_y;
double MouseHandler::yaw = 90.0;
double MouseHandler::pitch = 0.0;