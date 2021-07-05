#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

//#include <glt_transform.hh>

#include "camera.hh"
#include "image.hh"
#include "image_io.hh"
#include "object_vbo.hh"
#include "mouse_handler.hh"
#include "keyboard_handler.hh"
#include "model.hh"
#include "renderer.hh"
#include "main_shader.hh"

void window_resize(int width, int height)
{
    glViewport(0, 0, width, height);
    TEST_OPENGL_ERROR();
}

void display()
{
    // render_light_position();
    Renderer::render_camera_position();
}

void init_glut(int& argc, char* argv[])
{
    // glewExperimental = GL_TRUE;
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Shader Programming");
    glutDisplayFunc(display);
    glutSpecialFunc(KeyboardHandler::handle_keyboard);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutPassiveMotionFunc(MouseHandler::handle_mouse);
    glutReshapeFunc(window_resize);
}

bool init_glew()
{
    if (glewInit())
    {
        std::cerr << " Error while initializing glew";
        return false;
    }
    return true;
}

void init_GL()
{
    glEnable(GL_DEPTH_TEST);
    TEST_OPENGL_ERROR();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    TEST_OPENGL_ERROR();
    glEnable(GL_CULL_FACE);
    TEST_OPENGL_ERROR();
    glClearColor(0.4, 0.4, 0.4, 1.0);
    TEST_OPENGL_ERROR();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
}

void init_lights()
{
    lights[0] = {1.0, 1.0, 2.0};

    GLint loc = glGetUniformLocation(program_id, "light_position");
    if (loc != -1)
        glUniform3f(loc, lights[0][0], lights[0][1], lights[0][2]);
}

void init_buffers()
{
    glGenFramebuffers(1, &depth_map_FBO);

    unsigned int depth_map;
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH,
                 SHADOW_HEIGHT,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_FLOAT,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           depth_map,
                           0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main(int argc, char* argv[])
{
    init_glut(argc, argv);
    if (!init_glew())
        std::exit(-1);
    init_GL();

    main_shader::init_shaders();

    std::shared_ptr<scene::Model> model =
        scene::Model::from_file("obj/cube.obj");

    main_shader::init_object_vbo(model);
    camera.update_camera(camera);

    // init_lights();

    // init_textures();
    glutMainLoop();
    return 0;
}
