#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

constexpr int window_width = 1024;
constexpr int window_height = 1024;
constexpr int window_center_x = 1024 / 2;
constexpr int window_center_y = 1024 / 2;
constexpr unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

GLuint program_id;
GLuint model_vao_id;
unsigned int depth_map_FBO;
glm::vec3 lights[1];