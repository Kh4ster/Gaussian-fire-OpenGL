#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cassert>

#include <glm/glm.hpp>

namespace scene
{
/**
 * A triangle has three components (noted x, y, z)
 */

template <typename vec>
struct Triangle
{
    vec x;
    vec y;
    vec z;
};

class Model
{
  public:
    Model() = default;
    Model(const std::vector<Triangle<glm::vec3>>& vertices,
          const std::vector<Triangle<glm::vec3>>& normals,
          const std::vector<Triangle<glm::vec2>>& textures);

    static Model from_file(const std::string& path);

    // Size is the same for vertices and normals
    std::size_t size() const
    {
        assert(normals_.size() == vertices_.size());
        return vertices_.size() * sizeof(Triangle<glm::vec3>);
    }

    glm::vec3* const get_vertices()
    {
        return reinterpret_cast<glm::vec3*>(vertices_.data());
    };

    glm::vec3* const get_normals()
    {
        return reinterpret_cast<glm::vec3*>(normals_.data());
    };

  private:
    std::vector<Triangle<glm::vec3>> vertices_;
    std::vector<Triangle<glm::vec3>> normals_;
    std::vector<Triangle<glm::vec2>> textures_;
};

static Model main_model;

} // namespace scene