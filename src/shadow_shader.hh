#pragma once

#include "shader_handler.hh"
#include "program_propreties.hh"

namespace shadow_shader
{
void init_fbo()
{
    // configure depth map FBO
    // Generate buffers
    glGenFramebuffers(1, &depth_map_FBO);

    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH,
                 SHADOW_HEIGHT,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_FLOAT,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Bind buffers
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           depth_map,
                           0);
    // We only need the depth information when rendering the scene from the
    // light's perspective so there is no need for a color buffer. A framebuffer
    // object however is not complete without a color buffer so we need to
    // explicitly tell OpenGL we're not going to render any color data
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void init_object_vbo()
{
    int max_nb_vbo = 1;
    int nb_vbo = 0;
    int index_vbo = 0;
    GLuint vbo_ids[max_nb_vbo];

    GLint vertex_location = glGetAttribLocation(program_id, "position");
    TEST_OPENGL_ERROR();

    glGenVertexArrays(1, &model_shadow_vao_id);
    TEST_OPENGL_ERROR();
    glBindVertexArray(model_vao_id);
    TEST_OPENGL_ERROR();

    if (vertex_location != -1)
        nb_vbo++;

    glGenBuffers(nb_vbo, vbo_ids);
    TEST_OPENGL_ERROR();

    if (vertex_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER,
                     scene::main_model.size(),
                     scene::main_model.get_vertices(),
                     GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(vertex_location);
        TEST_OPENGL_ERROR();
    }
}

bool init_shaders()
{
    return shader::init_shaders("shaders/shadow_vertex.shd",
                                "shaders/shadow_fragment.shd",
                                shadow_program_id);
}
} // namespace shadow_shader
