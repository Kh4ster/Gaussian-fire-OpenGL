#pragma once

#include <glm/glm.hpp>
#include <chrono>
#include <ctime>
#include <GL/freeglut.h>

#include "mouse_handler.hh"
#include "program_propreties.hh"
#include "camera.hh"

struct KeyboardHandler
{
    static std::chrono::duration<float> delta;
    static std::chrono::time_point<std::chrono::system_clock> last_frame;

    static constexpr int delta_x = 10;
    static constexpr int delta_y = 10;

    static void handle_keyboard(int key, int x, int y)
    {
        // Compute elapsed time
        auto current_frame = std::chrono::system_clock::now();
        delta = current_frame - last_frame;
        last_frame = current_frame;

        float camera_speed = std::min(scene::camera.base_speed * delta.count(),
                                      scene::camera.base_speed * 0.05f);

        switch (key)
        {
        case GLUT_KEY_UP:
            scene::camera.origin_ += camera_speed * scene::camera.target_;
            break;
        case GLUT_KEY_DOWN:
            scene::camera.origin_ -= camera_speed * scene::camera.target_;
            break;
        case GLUT_KEY_LEFT:
            scene::camera.origin_ -=
                glm::normalize(
                    glm::cross(scene::camera.target_, scene::camera.up_)) *
                camera_speed;
            break;
        case GLUT_KEY_RIGHT:
            scene::camera.origin_ +=
                glm::normalize(
                    glm::cross(scene::camera.target_, scene::camera.up_)) *
                camera_speed;
            break;
        case GLUT_KEY_F1:
            glutDestroyWindow(glutGetWindow());
            exit(0);
            break;
        default:
            return;
        }

        scene::camera.update_camera();
        glutPostRedisplay();
    }

    static void normal_key_handler(unsigned char key, int x, int y)
    {
        switch (key)
        {
        case 'w':
            // TODO
            MouseHandler::handle_mouse(MouseHandler::last_x,
                                       MouseHandler::last_y - delta_y);
            break;
        case 'a':
            MouseHandler::handle_mouse(MouseHandler::last_x - delta_x,
                                       MouseHandler::last_y);
            break;
        case 's':
            MouseHandler::handle_mouse(MouseHandler::last_x,
                                       MouseHandler::last_y + delta_y);
            break;
        case 'd':
            MouseHandler::handle_mouse(MouseHandler::last_x + delta_x,
                                       MouseHandler::last_y);
            break;
        default:
            return;
        }
    }
};

std::chrono::time_point<std::chrono::system_clock> KeyboardHandler::last_frame =
    std::chrono::system_clock::now();
std::chrono::duration<float> KeyboardHandler::delta;