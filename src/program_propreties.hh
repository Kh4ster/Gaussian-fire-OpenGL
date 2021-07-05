#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

// Main shader
GLuint program_id;
GLuint model_vao_id;

constexpr int window_width = 1024;
constexpr int window_height = 1024;
constexpr int window_center_x = 1024 / 2;
constexpr int window_center_y = 1024 / 2;

// Shadow shader
GLuint depth_map_FBO;
GLuint depth_map;
glm::vec3 lights[1];
float strength_light[1];

constexpr unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;