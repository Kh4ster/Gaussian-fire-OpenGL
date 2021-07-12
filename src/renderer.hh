#pragma once

#include <GL/freeglut.h>

#include "program_propreties.hh"
#include "object_vbo.hh"
#include "camera.hh"
#include "shader_handler.hh"
#include "model.hh"
#include "light.hh"

namespace Renderer
{

static void render_scene(GLuint model_vao)
{
    glBindVertexArray(model_vao);
    TEST_OPENGL_ERROR();
    glDrawArrays(GL_TRIANGLES, 0, scene::main_model.size());
    TEST_OPENGL_ERROR();
    glBindVertexArray(0);
    TEST_OPENGL_ERROR();
}

static void render_shadow()
{
    glUseProgram(shadow_program_id);
    TEST_OPENGL_ERROR();

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    TEST_OPENGL_ERROR();
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
    TEST_OPENGL_ERROR();
    glClear(GL_DEPTH_BUFFER_BIT);
    TEST_OPENGL_ERROR();

    const glm::mat4 light_projection_matrix =
        glm::ortho(-10.0f,
                   10.0f,
                   -10.0f,
                   10.0f,
                   scene::camera.z_min_, // FIXME?
                   scene::camera.z_max_);
    // Look from the light position
    // to the middle of the scene
    // up vector same as camera
    const glm::mat4 light_view_matrix = glm::lookAt(scene::main_light.origin_,
                                                    glm::vec3(0.f, 0.f, 0.f),
                                                    scene::camera.up_);

    GLint loc = glGetUniformLocation(shadow_program_id, "model_view_matrix");
    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(light_view_matrix));

    loc = glGetUniformLocation(shadow_program_id, "projection_matrix");
    if (loc != -1)
        glUniformMatrix4fv(loc,
                           1,
                           GL_FALSE,
                           glm::value_ptr(light_projection_matrix));
    TEST_OPENGL_ERROR();

    render_scene(model_shadow_vao_id);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    TEST_OPENGL_ERROR();
}

static void render_camera_position()
{
    glUseProgram(program_id);
    TEST_OPENGL_ERROR();

    // reset viewport
    glViewport(0, 0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TEST_OPENGL_ERROR();

    render_scene(model_vao_id);
}

static void render_quad()
{
    glUseProgram(quad_program_id);
    TEST_OPENGL_ERROR();

    glViewport(0, 0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TEST_OPENGL_ERROR();

    GLint loc = glGetUniformLocation(quad_program_id, "near_plane");
    if (loc != -1)
        glUniform1f(loc, scene::camera.z_min_);
    loc = glGetUniformLocation(quad_program_id, "far_plane");
    if (loc != -1)
        glUniform1f(loc, scene::camera.z_max_);
    TEST_OPENGL_ERROR();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    TEST_OPENGL_ERROR();

    GLuint quad_vbo;
    if (quad_vao == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quad_vao);
        glGenBuffers(1, &quad_vbo);
        glBindVertexArray(quad_vao);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(quadVertices),
                     &quadVertices,
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              5 * sizeof(float),
                              (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              5 * sizeof(float),
                              (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quad_vao);
    TEST_OPENGL_ERROR();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    TEST_OPENGL_ERROR();
}

void display()
{
    TEST_OPENGL_ERROR();
    render_shadow();
    TEST_OPENGL_ERROR();
    // render_camera_position();
    render_quad();
    TEST_OPENGL_ERROR();
    glutSwapBuffers();
}
} // namespace Renderer