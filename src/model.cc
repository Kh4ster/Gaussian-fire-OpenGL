#include "model.hh"
#include <fstream>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <sstream>

namespace scene
{

Model::Model(const std::vector<Triangle<glm::vec3>>& vertices,
             const std::vector<Triangle<glm::vec3>>& normals,
             const std::vector<Triangle<glm::vec2>>& textures)
    : vertices_(vertices)
    , normals_(normals)
    , textures_(textures)
{
    assert(vertices_.size() == normals_.size());
    assert(vetices_.size() == textures_.size());
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
    std::vector<glm::vec2> all_textures;

    // List of all vertices stored as face
    std::vector<Triangle<glm::vec3>> vertices;
    // List of all normals stored as face (containing the normals)
    std::vector<Triangle<glm::vec3>> normals;
    // List of all textures coordinates stored as face
    std::vector<Triangle<glm::vec2>> textures;

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
        else if (start == "vt")
        {
            std::istringstream ss(line.substr(2));
            glm::vec2 texture_coord;
            ss >> texture_coord.x;
            ss >> texture_coord.y;
            all_textures.push_back(texture_coord);
        }
        else if (start == "f ")
        {
            // line = `f 1/2/3 1/2/3 1/2/3`
            unsigned int vertex1, vertex2, vertex3;
            unsigned int texture1, texture2, texture3;
            unsigned int normal1, normal2, normal3;
            sscanf(line.c_str(),
                   "f %i/%i/%i %i/%i/%i %i/%i/%i",
                   &vertex1,
                   &texture1,
                   &normal1,
                   &vertex2,
                   &texture2,
                   &normal2,
                   &vertex3,
                   &texture3,
                   &normal3);

            // Vertices
            Triangle<glm::vec3> face_vertices = {all_vertices[vertex1 - 1],
                                                 all_vertices[vertex2 - 1],
                                                 all_vertices[vertex3 - 1]};
            vertices.push_back(face_vertices);

            // Normals
            Triangle<glm::vec3> face_normals = {all_normals[normal1 - 1],
                                                all_normals[normal2 - 1],
                                                all_normals[normal3 - 1]};
            normals.push_back(face_normals);

            // Textures
            Triangle<glm::vec2> face_texture = {all_textures[texture1 - 1],
                                                all_textures[texture2 - 1],
                                                all_textures[texture3 - 1]};
            textures.push_back(face_texture);
        }
        // Everything else is ignored.
    }

    return Model(vertices, normals, textures);
}
} // namespace scene