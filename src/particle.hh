#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <random>

namespace particle
{
struct Particle
{
    Particle(const glm::vec3 _position,
             const glm::vec3 _velocity,
             const float _life,
             const glm::vec4 _color)
        : position(_position)
        , velocity(_velocity)
        , life(_life)
        , color(_color)
    {
    }

    Particle() = default;

    ~Particle() = default;

    static constexpr float base_life = 1.f;
    static constexpr float delta = 0.1f;

    glm::vec3 position = {0.f, 0.f, 0.f};
    glm::vec3 velocity = {0.f, 0.f, 0.f};
    float life = 0.f;
    glm::vec4 color = {1.f, 0.f, 0.f, 0.f};
};

class ParticleGenerator
{
  public:
    ParticleGenerator() = default;
    ParticleGenerator(glm::vec3 velocity,
                      glm::vec3 position,
                      GLuint texture_id,
                      const unsigned int amount)
        : velocity_(velocity)
        , position_(position)
    {
        this->init(amount);
    }

    void update(const unsigned int nb_new_particle = 2)
    {
        // Find dead particles
        // Respawn the given number of particles
        unsigned int respawned_particle = 0;
        for (Particle& particle : particles_)
        {
            if (particle.life <= 0.f)
            {
                respawn(particle);
                respawned_particle++;
                if (respawned_particle == nb_new_particle)
                    break;
            }
        }

        // Update all particles
        for (Particle& particle : particles_)
        {
            particle.life -= Particle::delta;
            if (particle.life > 0.0f)
            {
                particle.position -= particle.velocity * Particle::delta;
                particle.color.x -= Particle::delta * 2.5f;
            }
        }
    }

    GLuint get_vao() const { return VAO_; }

    GLuint get_texture() const { return texture_id_; }

    const std::vector<Particle>& get_particles() const { return particles_; }

  private:
    void init(const unsigned int nb_particles)
    {
        // xy -> coordinates
        // zw -> texture coordinates
        // clang-format off
        float particle_quad[] = {
            #include "../obj/cube.nico"
            };
        // clang-format on

        glGenVertexArrays(1, &VAO_);
        TEST_OPENGL_ERROR();
        glBindVertexArray(VAO_);
        TEST_OPENGL_ERROR();

        GLuint vbo_id;
        glGenBuffers(1, &vbo_id);
        TEST_OPENGL_ERROR();

        GLint vertex_location = glGetAttribLocation(fire_program_id, "vertex");
        TEST_OPENGL_ERROR();

        // fill mesh buffer
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(particle_quad),
                     particle_quad,
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

        particles_ = std::vector<Particle>(nb_particles);
    }

    void respawn(Particle& particle,
                 const glm::vec3 offset = glm::vec3(0.f, 0.f, 0.f))
    {
        // const float random_life = ((rand() % 4) + Particle::base_life);
        // const float random_x = distribution(generator);
        // const float random_y = distribution(generator);

        // particle.position =
        //    position_ + glm::vec3(random_x, random_y, 1.f) + offset;
        // particle.velocity = velocity_ * 0.1f;
        // particle.life = random_life;
        // particle.color = glm::vec4(1.f, 0.f, 0.f, 1.0f);
        const float random_pos = ((rand() % 100) - 50) / 10.0f;
        const float color = 0.5f + ((rand() % 100) / 100.0f);

        particle.position = position_ + random_pos + offset;
        particle.velocity = velocity_ * 0.1f;
        particle.life = Particle::base_life;
        particle.color = glm::vec4(color, color, color, 1.0f);
    }

  private:
    // state
    std::vector<Particle> particles_;

    // opengl variables
    GLuint VAO_;
    GLuint texture_id_;

    // settings generator
    glm::vec3 velocity_;
    glm::vec3 position_;

    // random generator
    // constexpr float mean = 5.0;
    // constexpr float std = 2.0;
    // std::default_random_engine generator;
    // std::normal_distribution<float> distribution(mean, std);
};

ParticleGenerator generator;
} // namespace particle