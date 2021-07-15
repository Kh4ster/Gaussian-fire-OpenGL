#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

// Main shader
GLuint program_id;
GLuint model_vao_id;
GLuint texture_id;

constexpr int window_width = 1024;
constexpr int window_height = 1024;
constexpr int window_center_x = 1024 / 2;
constexpr int window_center_y = 1024 / 2;

// Fire shader
GLuint fire_program_id;
GLuint fire_vao;