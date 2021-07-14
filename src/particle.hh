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

    static constexpr float base_life = 5.f;
};

class ParticleGenerator
{
  public:
    ParticleGenerator() = default;
    ParticleGenerator(const glm::vec3& direction,
                      const glm::vec3& position,
                      const float speed,
                      const unsigned int nb_new_particles,
                      const unsigned int amount)
        : direction_(direction)
        , position_(position)
        , speed_(speed)
        , nb_new_particles_(nb_new_particles)
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
                const float new_color_g =
                    std::max(particle.color.g - 0.07f, 0.0f);
                particle.color =
                    glm::vec4(particle.color.r, new_color_g, 0.f, 1.f);
            }
        }
    }

    const std::vector<Particle>& get_particles() const { return particles_; }

  private:
    void init(const unsigned int nb_particles)
    {
        // Create dead particles
        particles_ = std::vector<Particle>(nb_particles);
    }

    void respawn(Particle& particle,
                 const glm::vec3 offset = glm::vec3(0.f, 0.f, 0.f))
    {
        const float random_life =
            life_distribution_(rnd_engine_) + Particle::base_life;

        const float random_r = pos_distribution_(rnd_engine_);
        const float random_teta = (rand() % 100) / 100.f * 2 * M_PI;
        const float random_x = random_r * std::cos(random_teta);
        const float random_z = random_r * std::sin(random_teta);

        particle.position =
            position_ + glm::vec3(random_x, 0, random_z) + offset;
        const float noise = noise_(rnd_engine_);
        particle.direction = direction_; // + noise?
        particle.speed = speed_ + noise;
        particle.life = random_life;
        particle.color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // hardcoded so far
    }

  private:
    // state
    std::vector<Particle> particles_;

    // settings generator
    glm::vec3 direction_;
    float speed_;
    glm::vec3 position_;
    unsigned int nb_new_particles_; // new particle to spawn at every update

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

ParticleGenerator generator;
} // namespace particle