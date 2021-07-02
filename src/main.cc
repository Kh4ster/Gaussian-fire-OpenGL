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
#include "shadow_shader.hh"

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
    glutKeyboardFunc(KeyboardHandler::normal_key_handler);
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

int main(int argc, char* argv[])
{
    init_glut(argc, argv);
    if (!init_glew())
        std::exit(-1);
    init_GL();

    main_shader::init_shaders();

    scene::main_model = scene::Model::from_file("locker.obj");

    main_shader::init_object_vbo(model);
    camera.update_camera();

    lights[0] = {1.0, 1.5, 2.0};
    main_shader::init_lights();

    shadow_shader::init_fbo();

    // init_textures();
    glutMainLoop();
    return 0;
}
