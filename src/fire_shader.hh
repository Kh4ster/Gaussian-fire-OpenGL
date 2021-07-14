#pragma once

#include "program_propreties.hh"
#include "camera.hh"
#include "image.hh"
#include "particle.hh"
#include "shader_handler.hh"

namespace fire_shader
{

bool init_shaders()
{
    return shader::init_shaders("shaders/fire_vertex.shd",
                                "shaders/fire_fragment.shd",
                                fire_program_id);
}

void init_matrices(scene::Camera& camera)
{
    camera.update_camera(fire_program_id);
}

void init_cube_generator()
{
    // clang-format off
    // Vertex vertices
    float cube_vertices[] = {
        #include "../obj/cube.vertex"
    };
    // clang-format on

    glGenVertexArrays(1, &fire_vao);
    TEST_OPENGL_ERROR();
    glBindVertexArray(fire_vao);
    TEST_OPENGL_ERROR();

    GLuint vbo_ids[1];
    glGenBuffers(1, vbo_ids);
    TEST_OPENGL_ERROR();

    GLint vertex_location = glGetAttribLocation(fire_program_id, "vertex");
    TEST_OPENGL_ERROR();

    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
    TEST_OPENGL_ERROR();
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(cube_vertices),
                 cube_vertices,
                 GL_STATIC_DRAW);
    TEST_OPENGL_ERROR();
    // set mesh attributes
    glVertexAttribPointer(vertex_location,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(vertex_location);
    glBindVertexArray(0);
    TEST_OPENGL_ERROR();
}
} // namespace fire_shader