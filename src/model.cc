#include "model.hh"
#include <fstream>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <sstream>

namespace scene
{

Model::Model(const std::vector<Triangle>& triangles)
    : triangles_(triangles)
{
}

std::shared_ptr<Model> Model::from_file(const std::string& path)
{
    size_t pos_extension = path.find_last_of(".");
    if (pos_extension == std::string::npos ||
        path.substr(pos_extension) != ".obj")
    {
        std::cerr << "Error: " << path << " is not a `.obj` file\n";
        return nullptr;
    }

    std::ifstream in(path);

    // Vector of all vertices
    std::vector<glm::vec3> vertices;
    // List of all triangles (containing the vertices)
    std::vector<Triangle> triangles;

    std::string line;
    while (std::getline(in, line) && line.size() > 2)
    {
        const std::string start = line.substr(0, 2);
        if (start == "v ") // vertex
        {
            std::istringstream ss(line.substr(2));
            glm::vec3 vertex;
            ss >> vertex.x;
            ss >> vertex.y;
            ss >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (start == "f ")
        {
            Triangle triangle;
            unsigned int index_vertex;
            // First vertex
            std::stringstream ss(line.substr(2));
            ss >> index_vertex;
            triangle.x = vertices[index_vertex - 1];

            // Second vertex
            std::string str2(ss.str());
            size_t pos = str2.find(" ");
            std::stringstream ss2(str2.substr(pos));
            ss2 >> index_vertex;
            triangle.y = vertices[index_vertex - 1];

            // Third vertex
            pos = line.find_last_of(" ");
            std::stringstream ss3(line.substr(pos));
            ss3 >> index_vertex;
            triangle.z = vertices[index_vertex - 1];

            triangles.push_back(triangle);
        }
        // `vn` and `vt` ignored. Everything else is ignored.
    }

    return std::make_shared<Model>(triangles);
}
} // namespace scene