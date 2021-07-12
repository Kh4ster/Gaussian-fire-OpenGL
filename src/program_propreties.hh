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
GLuint shadow_program_id;
GLuint model_shadow_vao_id; // probably useless and use only one vao
GLuint depth_map_FBO;
GLuint depth_map;

constexpr unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

// Quad shader
GLuint quad_program_id;
GLuint quad_vao;