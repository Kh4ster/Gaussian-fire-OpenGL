#pragma once

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "program_propreties.hh"
#include "object_vbo.hh"
#include "camera.hh"
#include "shader_handler.hh"
#include "model.hh"
#include "light.hh"
#include "particle.hh"

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

static void render_camera_position()
{
    glUseProgram(program_id);
    TEST_OPENGL_ERROR();

    scene::camera.update_camera(program_id);

    // reset viewport
    glViewport(0, 0, window_width, window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TEST_OPENGL_ERROR();

    render_scene(model_vao_id);
    TEST_OPENGL_ERROR();
}

static void render_fire()
{
    glUseProgram(fire_program_id);
    TEST_OPENGL_ERROR();

    scene::camera.update_camera(fire_program_id);

    TEST_OPENGL_ERROR();

    // Update particles
    particle::generator.update();

    // Render every particles one by one
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    TEST_OPENGL_ERROR();
    for (const particle::Particle& particle :
         particle::generator.get_particles())
    {
        if (particle.life > 0.0f)
        {
            GLint loc = glGetUniformLocation(fire_program_id, "position");
            if (loc != -1)
            {
                glUniform3f(loc,
                            particle.position[0],
                            particle.position[1],
                            particle.position[2]);
            }
            loc = glGetUniformLocation(fire_program_id, "color");
            if (loc != -1)
            {
                glUniform4f(loc,
                            particle.color[0],
                            particle.color[1],
                            particle.color[2],
                            particle.color[3]);
            }
            TEST_OPENGL_ERROR();

            glBindVertexArray(particle::generator.get_vao());
            glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
            glBindVertexArray(0);
            TEST_OPENGL_ERROR();
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    TEST_OPENGL_ERROR();
}

void display()
{
    render_camera_position();
    render_fire();
    glutSwapBuffers();
}
} // namespace Renderer