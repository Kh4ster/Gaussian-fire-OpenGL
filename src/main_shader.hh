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
    int max_nb_vbo = 2; // FIXME: number of vbo
    int nb_vbo = 0;
    int index_vbo = 0;
    GLuint vbo_ids[max_nb_vbo];

    GLint vertex_location = glGetAttribLocation(program_id, "position");
    TEST_OPENGL_ERROR();
    GLint normal_location = glGetAttribLocation(program_id, "normal");
    TEST_OPENGL_ERROR();
    /*
    GLint normal_flat_location = glGetAttribLocation(program_id, "normalFlat");
    TEST_OPENGL_ERROR();
    GLint normal_smooth_location =
        glGetAttribLocation(program_id, "normalSmooth");
    TEST_OPENGL_ERROR();
    GLint color_location = glGetAttribLocation(program_id, "color");
    TEST_OPENGL_ERROR();
    GLint uv_location = glGetAttribLocation(program_id, "uv");
    TEST_OPENGL_ERROR();
    */

    glGenVertexArrays(1, &model_vao_id);
    TEST_OPENGL_ERROR();
    glBindVertexArray(model_vao_id);
    TEST_OPENGL_ERROR();

    if (vertex_location != -1)
        nb_vbo++;
    if (normal_location != -1)
        nb_vbo++;
    /*
    if (normal_flat_location != -1)
        nb_vbo++;
    if (normal_smooth_location != -1)
        nb_vbo++;
    if (color_location != -1)
        nb_vbo++;
    if (uv_location != -1)
        nb_vbo++;
    */
    glGenBuffers(nb_vbo, vbo_ids);
    TEST_OPENGL_ERROR();

    if (vertex_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER,
                     scene::main_model.size(),
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
                     scene::main_model.size(),
                     scene::main_model.get_normals(),
                     GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(normal_location);
        TEST_OPENGL_ERROR();
    }
    /*
    if (normal_flat_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER,
                     normal_flat_buffer_data.size() * sizeof(float),
                     normal_flat_buffer_data.data(),
                     GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(normal_flat_location,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(normal_flat_location);
        TEST_OPENGL_ERROR();
    }

    if (normal_smooth_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER,
                     normal_smooth_buffer_data.size() * sizeof(float),
                     normal_smooth_buffer_data.data(),
                     GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(normal_smooth_location,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(normal_smooth_location);
        TEST_OPENGL_ERROR();
    }

    if (color_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER,
                     color_buffer_data.size() * sizeof(float),
                     color_buffer_data.data(),
                     GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(color_location);
        TEST_OPENGL_ERROR();
    }

    if (uv_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        TEST_OPENGL_ERROR();
        glBufferData(GL_ARRAY_BUFFER,
                     uv_buffer_data.size() * sizeof(float),
                     uv_buffer_data.data(),
                     GL_STATIC_DRAW);
        TEST_OPENGL_ERROR();
        glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
        TEST_OPENGL_ERROR();
        glEnableVertexAttribArray(uv_location);
        TEST_OPENGL_ERROR();
    }*/

    glBindVertexArray(0);
}

void init_textures()
{
    tifo::rgb24_image* texture = tifo::load_image("data/texture.tga");
    tifo::rgb24_image* lighting = tifo::load_image("data/lighting.tga");
    tifo::rgb24_image* normalmap = tifo::load_image("data/normalmap.tga");
    GLuint texture_id;
    GLuint lighting_id;
    GLuint normalmap_id;
    GLint tex_location;
    GLint light_location;
    GLint normalmap_location;

    std::cout << "texture " << texture->sx << " ," << texture->sy << "\n";
    std::cout << "light " << lighting->sx << " ," << lighting->sy << "\n";
    std::cout << "normalmap " << normalmap->sx << " ," << normalmap->sy
              << std::endl;

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
    tex_location = glGetUniformLocation(program_id, "texture_sampler");
    TEST_OPENGL_ERROR();
    std::cout << "tex_location " << tex_location << std::endl;
    glUniform1i(tex_location, 0);
    TEST_OPENGL_ERROR();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    TEST_OPENGL_ERROR();

    glGenTextures(1, &lighting_id);
    TEST_OPENGL_ERROR();
    glActiveTexture(GL_TEXTURE1);
    TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, lighting_id);
    TEST_OPENGL_ERROR();
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 lighting->sx,
                 lighting->sy,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 lighting->pixels);
    TEST_OPENGL_ERROR();
    light_location = glGetUniformLocation(program_id, "lighting_sampler");
    TEST_OPENGL_ERROR();
    std::cout << "light_location " << light_location << std::endl;
    glUniform1i(light_location, 1);
    TEST_OPENGL_ERROR();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    TEST_OPENGL_ERROR();

    glGenTextures(1, &normalmap_id);
    TEST_OPENGL_ERROR();
    glActiveTexture(GL_TEXTURE2);
    TEST_OPENGL_ERROR();
    glBindTexture(GL_TEXTURE_2D, normalmap_id);
    TEST_OPENGL_ERROR();
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 normalmap->sx,
                 normalmap->sy,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 normalmap->pixels);
    TEST_OPENGL_ERROR();
    normalmap_location = glGetUniformLocation(program_id, "normalmap_sampler");
    TEST_OPENGL_ERROR();
    std::cout << "normalmap_location " << normalmap_location << std::endl;
    glUniform1i(normalmap_location, 2);
    TEST_OPENGL_ERROR();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    TEST_OPENGL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    TEST_OPENGL_ERROR();

    delete texture;
    delete lighting;
    delete normalmap;
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