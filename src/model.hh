#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace scene
{
/**
 * A triangle has three vertices (noted x, y, z)
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
    static std::shared_ptr<Model> from_file(const std::string& path);
    Model(const std::vector<Triangle>& triangles_);

    glm::vec3* const data()
    {
        return reinterpret_cast<glm::vec3*>(triangles_.data());
    };

    std::size_t size() const { return triangles_.size() * sizeof(Triangle); }

  private:
    Model() = default;
    std::vector<Triangle> triangles_;
};
} // namespace scene