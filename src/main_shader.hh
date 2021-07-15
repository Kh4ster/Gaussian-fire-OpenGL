#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <memory>
#include "model.hh"
#include "light.hh"
#include "program_propreties.hh"

namespace main_shader
{
void init_object_vbo()
{
    int max_nb_vbo = 3; // FIXME: number of vbo
    int nb_vbo = 0;
    int index_vbo = 0;
    GLuint vbo_ids[max_nb_vbo];

    GLint vertex_location = glGetAttribLocation(program_id, "position");
    TEST_OPENGL_ERROR();
    GLint normal_location = glGetAttribLocation(program_id, "normal");
    TEST_OPENGL_ERROR();
    GLint tex_location = glGetAttribLocation(program_id, "tex_coord");
    TEST_OPENGL_ERROR();

    glGenVertexArrays(1, &model_vao_id);
    TEST_OPENGL_ERROR();
    glBindVertexArray(model_vao_id);
    TEST_OPENGL_ERROR();

    if (vertex_location != -1)
        nb_vbo++;
    if (normal_location != -1)
        nb_vbo++;
    if (tex_location != -1)
        nb_vbo++;

    glGenBuffers(nb_vbo, vbo_ids);
    TEST_OPENGL_ERROR();

    if (vertex_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER,
                     scene::main_model.size_vertices(),
                     scene::main_model.get_vertices(),
                     GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(vertex_location);
        TEST_OPENGL_ERROR();
    }
    if (normal_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER,
                     scene::main_model.size_normals(),
                     scene::main_model.get_normals(),
                     GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(normal_location);
        TEST_OPENGL_ERROR();
    }
    if (tex_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER,
                     scene::main_model.size_textures(),
                     scene::main_model.get_textures(),
                     GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(tex_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(tex_location);
        TEST_OPENGL_ERROR();
    }

    glBindVertexArray(0);
}

void init_textures()
{
    tifo::rgb24_image* texture = tifo::load_image("data/wood-3.tga");

    std::cout << "texture " << texture->sx << " ," << texture->sy << "\n";

    GLint texture_units, combined_texture_units;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combined_texture_units);
    std::cout << "Limit 1 " << texture_units << " limit 2 "
              << combined_texture_units << std::endl;

    glGenTextures(1, &texture_id);
    TEST_OPENGL_ERROR();
    glActiveTexture(GL_TEXTURE0);
    TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    TEST_OPENGL_ERROR();
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 texture->sx,
                 texture->sy,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 texture->pixels);
    TEST_OPENGL_ERROR();
    GLint tex_location = glGetUniformLocation(program_id, "texture_sampler");
    TEST_OPENGL_ERROR();
    std::cout << "tex_location " << tex_location << std::endl;
    glUniform1i(tex_location, 0);
    TEST_OPENGL_ERROR();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete texture;
}

void init_lights()
{
    GLint loc = glGetUniformLocation(program_id, "light_position");
    if (loc != -1)
        glUniform3fv(loc,
                     scene::lights.size(),
                     glm::value_ptr(scene::lights.origins_get()[0]));
    loc = glGetUniformLocation(program_id, "strength_light");
    if (loc != -1)
        glUniform1fv(loc, scene::lights.size(), scene::lights.strengths_get());
    loc = glGetUniformLocation(program_id, "nb_lights");
    if (loc != -1)
        glUniform1i(loc, scene::lights.size());
}

bool init_shaders()
{
    return shader::init_shaders("shaders/main_vertex.shd",
                                "shaders/main_fragment.shd",
                                program_id);
}
} // namespace main_shader