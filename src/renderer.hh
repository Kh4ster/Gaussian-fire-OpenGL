#pragma once

#include <GL/freeglut.h>

#include "program_propreties.hh"
#include "object_vbo.hh"
#include "camera.hh"
#include "shader_handler.hh"
#include "model.hh"

struct Renderer
{
    static void render_vao()
    {
        glBindVertexArray(model_vao_id);
        TEST_OPENGL_ERROR();
        glDrawArrays(GL_TRIANGLES, 0, scene::main_model.size());
        TEST_OPENGL_ERROR();
        glBindVertexArray(0);
        TEST_OPENGL_ERROR();
    }

    static void render_light_position()
    {
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        TEST_OPENGL_ERROR();
        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
        TEST_OPENGL_ERROR();
        glClear(GL_DEPTH_BUFFER_BIT);
        TEST_OPENGL_ERROR();

        auto old = camera.origin_;
        camera.origin_ = lights[0];
        render_vao();
        camera.origin_ = old;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        TEST_OPENGL_ERROR();
        glViewport(0, 0, window_width, window_height);
        TEST_OPENGL_ERROR();
    }

    static void render_camera_position()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TEST_OPENGL_ERROR();

        render_vao();

        glutSwapBuffers();
    }
};