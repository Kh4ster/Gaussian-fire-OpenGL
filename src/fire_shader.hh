#pragma once

#include "shader_handler.hh"
#include "program_propreties.hh"
#include "camera.hh"
#include "image.hh"

namespace fire_shader
{

bool init_shaders()
{
    return shader::init_shaders("shaders/fire_vertex.shd",
                                "shaders/fire_fragment.shd",
                                fire_program_id);
}

GLuint init_texture()
{
    tifo::rgb24_image* fire_texture = tifo::load_image("data/fire.tga");
    GLuint texture_id;

    glGenTextures(1, &texture_id);
    TEST_OPENGL_ERROR();
    glActiveTexture(GL_TEXTURE0);
    TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    TEST_OPENGL_ERROR();
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 fire_texture->sx,
                 fire_texture->sy,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 fire_texture->pixels);
    TEST_OPENGL_ERROR();
    GLint tex_location = glGetUniformLocation(fire_program_id, "sampler");
    TEST_OPENGL_ERROR();
    glUniform1i(tex_location, 0);
    TEST_OPENGL_ERROR();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    TEST_OPENGL_ERROR();

    return texture_id;
}

void init_matrices(scene::Camera& camera)
{
    camera.update_camera(fire_program_id);
}

} // namespace fire_shader