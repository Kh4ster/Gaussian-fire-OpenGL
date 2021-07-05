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
struct Triangle
{
    glm::vec3 x;
    glm::vec3 y;
    glm::vec3 z;
};

class Model
{
  public:
    Model(const std::vector<Triangle>& vertices_,
          const std::vector<Triangle>& normals);

    static std::shared_ptr<Model> from_file(const std::string& path);

    // Size is the same for vertices and normals
    std::size_t size() const
    {
        assert(normals_.size() == vertices_.size());
        return vertices_.size() * sizeof(Triangle);
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
    Model() = default;
    std::vector<Triangle> vertices_;
    std::vector<Triangle> normals_;
};
} // namespace scene