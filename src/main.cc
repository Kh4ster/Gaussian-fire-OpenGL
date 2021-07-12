#include <GL/glew.h>
#include <GL/freeglut.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

#include "image.hh"
#include "image_io.hh"
#include "mouse_handler.hh"
#include "keyboard_handler.hh"
#include "renderer.hh"
#include "main_shader.hh"
#include "shadow_shader.hh"
#include "light.hh"
#include "model.hh"
#include "particle.hh"

void window_resize(int width, int height)
{
    glViewport(0, 0, width, height);
    TEST_OPENGL_ERROR();
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
    glutDisplayFunc(Renderer::display);
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

    //------- Init Main shader
    main_shader::init_shaders();

    scene::main_model = scene::Model::from_file("obj/cube_plan.obj");

    main_shader::init_object_vbo();
    scene::camera.update_camera();

    // x -> front to back
    // y -> top to bottom
    // z -> left to right
    const glm::vec3 light_origin = {-3.0, 5.0, -3.0};
    scene::main_light = scene::Light(light_origin, 3.f);
    main_shader::init_lights();
    // main_shader::init_textures();

    //------- Init shadow shader
    shadow_shader::init_shaders();
    shadow_shader::init_fbo();
    shadow_shader::init_object_vbo();

    // fire_shader::init_shaders();
    // ParticleGenerator particle_generator{glm::vec2(1.f, 2.f),
    // glm::vec2(0.f, 0.f)

    // init_textures();
    glutMainLoop();
    return 0;
}
