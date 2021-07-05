#pragma once

#include <string>
#include <iostream>
#include <fstream>

#define TEST_OPENGL_ERROR()                                                    \
    do                                                                         \
    {                                                                          \
        GLenum err = glGetError();                                             \
        if (err != GL_NO_ERROR)                                                \
            std::cerr << "OpenGL ERROR!" << __LINE__ << std::endl;             \
    } while (0)

namespace shader
{
std::string load(const std::string& filename)
{
    std::ifstream input_src_file(filename, std::ios::in);
    std::string ligne;
    std::string file_content = "";
    if (input_src_file.fail())
    {
        std::cerr << "FAIL\n";
        return "";
    }
    while (getline(input_src_file, ligne))
    {
        file_content = file_content + ligne + "\n";
    }
    file_content += '\0';
    input_src_file.close();
    return file_content;
}

bool init_shaders(const std::string& vertex_path,
                  const std::string& fragment_path,
                  GLuint id)
{
    std::string vertex_src = shader::load(vertex_path.c_str());
    std::string fragment_src = shader::load(fragment_path.c_str());
    GLuint shader_id[2];
    GLint compile_status = GL_TRUE;
    char* vertex_shd_src =
        (char*)std::malloc(vertex_src.length() * sizeof(char));
    char* fragment_shd_src =
        (char*)std::malloc(fragment_src.length() * sizeof(char));
    vertex_src.copy(vertex_shd_src, vertex_src.length());
    fragment_src.copy(fragment_shd_src, fragment_src.length());

    shader_id[0] = glCreateShader(GL_VERTEX_SHADER);
    TEST_OPENGL_ERROR();
    shader_id[1] = glCreateShader(GL_FRAGMENT_SHADER);
    TEST_OPENGL_ERROR();

    glShaderSource(shader_id[0], 1, (const GLchar**)&(vertex_shd_src), 0);
    TEST_OPENGL_ERROR();
    glShaderSource(shader_id[1], 1, (const GLchar**)&(fragment_shd_src), 0);
    TEST_OPENGL_ERROR();
    for (int i = 0; i < 2; i++)
    {
        glCompileShader(shader_id[i]);
        TEST_OPENGL_ERROR();
        glGetShaderiv(shader_id[i], GL_COMPILE_STATUS, &compile_status);
        if (compile_status != GL_TRUE)
        {
            GLint log_size;
            char* shader_log;
            glGetShaderiv(shader_id[i], GL_INFO_LOG_LENGTH, &log_size);
            shader_log = (char*)std::malloc(
                log_size + 1); /* +1 pour le caractere de fin de chaine '\0' */
            if (shader_log != 0)
            {
                glGetShaderInfoLog(shader_id[i],
                                   log_size,
                                   &log_size,
                                   shader_log);
                std::cerr << "SHADER " << i << ": " << shader_log << std::endl;
                std::free(shader_log);
            }
            std::free(vertex_shd_src);
            std::free(fragment_shd_src);
            glDeleteShader(shader_id[0]);
            glDeleteShader(shader_id[1]);
            return false;
        }
    }
    std::free(vertex_shd_src);
    std::free(fragment_shd_src);

    GLint link_status = GL_TRUE;
    id = glCreateProgram();
    TEST_OPENGL_ERROR();
    if (id == 0)
        return false;
    for (int i = 0; i < 2; i++)
    {
        glAttachShader(id, shader_id[i]);
        TEST_OPENGL_ERROR();
    }
    glLinkProgram(id);
    TEST_OPENGL_ERROR();
    glGetProgramiv(id, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
    {
        GLint log_size;
        char* program_log;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(
            log_size + 1); /* +1 pour le caractere de fin de chaine '\0' */
        if (program_log != 0)
        {
            glGetProgramInfoLog(id, log_size, &log_size, program_log);
            std::cerr << "Program " << program_log << std::endl;
            std::free(program_log);
        }
        glDeleteProgram(id);
        TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[0]);
        TEST_OPENGL_ERROR();
        glDeleteShader(shader_id[1]);
        TEST_OPENGL_ERROR();
        id = 0;
        return false;
    }
    glUseProgram(id);
    TEST_OPENGL_ERROR();
    return true;
}

} // namespace shader