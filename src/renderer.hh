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
        glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
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
    glUseProgram(program_id);
    TEST_OPENGL_ERROR();

    render_scene(model_vao_id);
}

void display()
{
    render_shadow();
    render_camera_position();
    glutSwapBuffers();
}
} // namespace Renderer