#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <cmath>

namespace particle
{
struct Particle
{
    Particle(const glm::vec3 _position,
             const glm::vec3 _direction,
             const float _speed,
             const float _life,
             const glm::vec4 _color)
        : position(_position)
        , direction(_direction)
        , speed(_speed)
        , life(_life)
        , color(_color)
    {
    }

    Particle() = default;

    ~Particle() = default;

    glm::vec3 position = {0.f, 0.f, 0.f};
    glm::vec3 direction = {0.f, 0.f, 0.f};
    float speed = 1.f;
    float life = 0.f;
    glm::vec4 color = {1.f, 0.f, 0.f, 0.f};

    float base_life_ = 5.f;
};

class ParticleGenerator
{
  public:
    ParticleGenerator() = default;
    ParticleGenerator(const glm::vec3& direction,
                      const glm::vec3& position,
                      const glm::vec4& color,
                      const float speed,
                      const unsigned int nb_new_particles,
                      GLuint texture_id,
                      const unsigned int amount,
                      const bool circle = false,
                      const bool color_attenuation = true,
                      const float base_life = 5.f)
        : direction_(direction)
        , position_(position)
        , color_(color)
        , speed_(speed)
        , nb_new_particles_(nb_new_particles)
        , texture_id_(texture_id)
        , circle_(circle)
        , color_attenuation_(color_attenuation)
        , base_life_(base_life)
    {
        this->init(amount);
    }

    void update()
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
                if (respawned_particle == nb_new_particles_)
                    break;
            }
        }

        // Update all particles
        for (Particle& particle : particles_)
        {
            particle.life -= 1.f;
            if (particle.life > 0.f)
            {
                particle.position += particle.direction * particle.speed;
                if (color_attenuation_)
                {
                    const float new_color_g =
                        std::max(particle.color.g - 0.07f, 0.0f);
                    particle.color =
                        glm::vec4(particle.color.r, new_color_g, 0.f, 1.f);
                }
                else
                    particle.color = color_;
            }
        }
    }

    void set_position(const glm::vec3 position) { position_ = position; }

    GLuint get_vao() const { return VAO_; }

    GLuint get_texture() const { return texture_id_; }

    void activate()
    {
        speed_ += 0.005f;
        color_ += 0.005f;
    }

    glm::vec3 get_position() const { return position_; }

    const std::vector<Particle>& get_particles() const { return particles_; }

  private:
    void init(const unsigned int nb_particles)
    {
        // clang-format off
        // Vertex vertices
        float cube_vertices[] = {
            #include "../obj/cube.vertex"
        };
        // clang-format on

        glGenVertexArrays(1, &VAO_);
        TEST_OPENGL_ERROR();
        glBindVertexArray(VAO_);
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

        // Create dead particles
        particles_ = std::vector<Particle>(nb_particles);
    }

    void respawn(Particle& particle,
                 const glm::vec3 offset = glm::vec3(0.f, 0.f, 0.f))
    {
        const float random_life = life_distribution_(rnd_engine_) + base_life_;

        float random_r;
        if (circle_)
            random_r = 1.f;
        else
            random_r = pos_distribution_(rnd_engine_);
        const float random_teta = (rand() % 100) / 100.f * 2 * M_PI;
        const float random_x = random_r * std::cos(random_teta);
        const float random_z = random_r * std::sin(random_teta);

        particle.position =
            position_ + glm::vec3(random_x, 0, random_z) + offset;
        const float noise = noise_(rnd_engine_);
        particle.direction = direction_; // + noise?
        particle.speed = speed_ + noise;
        particle.life = random_life;
        particle.color = color_; // hardcoded so far
    }

  private:
    // state
    std::vector<Particle> particles_;

    // opengl variables
    GLuint VAO_;
    GLuint texture_id_;

    // settings generator
    glm::vec3 direction_;
    float speed_;
    glm::vec3 position_;
    unsigned int nb_new_particles_; // new particle to spawn at every update
    glm::vec4 color_;
    bool circle_;
    bool color_attenuation_;
    float base_life_;

    // random generators
    // Position generator
    static constexpr float pos_mean_ = 0.f;
    static constexpr float pos_std_ = 0.25f;
    std::normal_distribution<float> pos_distribution_{pos_mean_, pos_std_};

    // Life generator
    static constexpr float life_mean_ = 0.f;
    static constexpr float life_std_ = 5.f;
    std::normal_distribution<float> life_distribution_{life_mean_, life_std_};

    // Uniform noise
    std::uniform_real_distribution<float> noise_{-0.05f, 0.05f};

    std::default_random_engine rnd_engine_;
};

ParticleGenerator fire_generator;
ParticleGenerator portal_generator_A;
ParticleGenerator portal_generator_B;

} // namespace particle