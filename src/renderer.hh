#pragma once

#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "program_propreties.hh"
#include "object_vbo.hh"
#include "camera.hh"
#include "model.hh"
#include "light.hh"
#include "particle.hh"
#include "shader_handler.hh"

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

static void render_generator(particle::ParticleGenerator& generator)
{
    glUseProgram(fire_program_id);
    TEST_OPENGL_ERROR();

    scene::camera.update_camera(fire_program_id);

    TEST_OPENGL_ERROR();

    // Update particles
    generator.update();

    // Render every particles one by one
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    TEST_OPENGL_ERROR();
    for (const particle::Particle& particle : generator.get_particles())
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
            loc = glGetUniformLocation(fire_program_id, "scale");
            if (loc != -1)
                glUniform1f(loc, generator.scale_get());

            glBindVertexArray(fire_vao);
            glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
            glBindVertexArray(0);
            TEST_OPENGL_ERROR();
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    TEST_OPENGL_ERROR();
}

void teleport()
{
    static int entering_zone = false;
    if (std::abs(scene::camera.origin_[0] -
                 particle::portal_generator_A.get_position()[0]) < 1.f &&
        std::abs(scene::camera.origin_[2] -
                 particle::portal_generator_A.get_position()[2]) < 1.f)
    {
        if (entering_zone < 200)
        {
            ++entering_zone;
            particle::portal_generator_A.activate();
        }
        else if (entering_zone == 200)
        {
            auto vect = particle::portal_generator_B.get_position();
            vect[1] = scene::camera.origin_[1];
            scene::camera.origin_ = vect;
        }
    }
}

void display()
{
    teleport();
    render_camera_position();
    render_generator(particle::fire_generator);
    render_generator(particle::portal_generator_A);
    render_generator(particle::portal_generator_B);
    glutSwapBuffers();
}
} // namespace Renderer