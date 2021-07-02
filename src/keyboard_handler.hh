#pragma once

#include <glm/glm.hpp>
#include <chrono>
#include <ctime>
#include <GL/freeglut.h>

#include "program_propreties.hh"
#include "camera.hh"

struct KeyboardHandler
{
    static std::chrono::duration<float> delta;
    static std::chrono::time_point<std::chrono::system_clock> last_frame;

    static void handle_keyboard(int key, int x, int y)
    {
        // Compute elapsed time
        auto current_frame = std::chrono::system_clock::now();
        delta = current_frame - last_frame;
        last_frame = current_frame;

        float camera_speed = std::min(camera.base_speed * delta.count(),
                                      camera.base_speed * 0.05f);

        switch (key)
        {
        case GLUT_KEY_UP:
            camera.origin_ += camera_speed * camera.target_;
            break;
        case GLUT_KEY_DOWN:
            camera.origin_ -= camera_speed * camera.target_;
            break;
        case GLUT_KEY_LEFT:
            camera.origin_ -=
                glm::normalize(glm::cross(camera.target_, camera.up_)) *
                camera_speed;
            break;
        case GLUT_KEY_RIGHT:
            camera.origin_ +=
                glm::normalize(glm::cross(camera.target_, camera.up_)) *
                camera_speed;
            break;
        case GLUT_KEY_F1:
            glutDestroyWindow(glutGetWindow());
            exit(0);
            break;
        default:
            return;
        }

        camera.update_camera(camera);
        glutPostRedisplay();
    }
};

std::chrono::time_point<std::chrono::system_clock> KeyboardHandler::last_frame =
    std::chrono::system_clock::now();
std::chrono::duration<float> KeyboardHandler::delta;