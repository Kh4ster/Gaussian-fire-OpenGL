#include "model.hh"
#include <fstream>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <sstream>

namespace scene
{

Model::Model(const std::vector<Triangle>& vertices,
             const std::vector<Triangle>& normals)
    : vertices_(vertices)
    , normals_(normals)
{
    assert(vertices_.size() == normals_.size());
}

Model Model::from_file(const std::string& path)
{
    size_t pos_extension = path.find_last_of(".");
    if (pos_extension == std::string::npos ||
        path.substr(pos_extension) != ".obj")
    {
        std::cerr << "Error: " << path << " is not a `.obj` file\n";
        return {};
    }

    std::ifstream in(path);
    if (!in)
    {
        std::cerr << "Error: No such file: " << path << "\n";
        return {};
    }

    // Vector of all vertices
    std::vector<glm::vec3> all_vertices;
    std::vector<glm::vec3> all_normals;

    // List of all vertices stored as face
    std::vector<Triangle> vertices;
    // List of all normals stored as face (containing the normals)
    std::vector<Triangle> normals;

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
            all_vertices.push_back(vertex);
        }
        else if (start == "vn")
        {
            std::istringstream ss(line.substr(2));
            glm::vec3 normal;
            ss >> normal.x;
            ss >> normal.y;
            ss >> normal.z;
            all_normals.push_back(normal);
        }
        else if (start == "f ")
        {
            // line = `f 1/2/3 1/2/3 1/2/3`
            Triangle face_vertices;
            Triangle face_normals;

            std::istringstream ss(line.substr(2));
            std::string token;
            // first point
            std::getline(ss, token, ' ');
            {
                std::istringstream local_ss(token);
                std::string trash;
                unsigned int vertex_index;
                local_ss >> vertex_index;
                local_ss >> trash; // skip `/`
                unsigned int normal_index;
                local_ss >> normal_index;

                // index start at 1 in .obj file
                face_vertices.x = all_vertices[vertex_index - 1];
                face_normals.x = all_normals[normal_index - 1];
            }

            // second point
            std::getline(ss, token, ' ');
            {
                std::istringstream local_ss(token);
                std::string trash;
                unsigned int vertex_index;
                local_ss >> vertex_index;
                local_ss >> trash; // skip `/`
                unsigned int normal_index;
                local_ss >> normal_index;

                face_vertices.y = all_vertices[vertex_index - 1];
                face_normals.y = all_normals[normal_index - 1];
            }

            // third point
            std::getline(ss, token, ' ');
            {
                std::istringstream local_ss(token);
                std::string trash;
                unsigned int vertex_index;
                local_ss >> vertex_index;
                local_ss >> trash; // skip `/`
                unsigned int normal_index;
                local_ss >> normal_index;

                face_vertices.z = all_vertices[vertex_index - 1];
                face_normals.z = all_normals[normal_index - 1];
            }

            vertices.push_back(face_vertices);
            normals.push_back(face_normals);
        }
        // vt` ignored. Everything else is ignored.
    }

    return Model(vertices, normals);
}
} // namespace scene