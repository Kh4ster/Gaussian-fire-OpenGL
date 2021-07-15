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
#include "light.hh"
#include "model.hh"
#include "particle.hh"
#include "fire_shader.hh"
#include "shader_handler.hh"

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

void timer(int extra)
{
    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

int main(int argc, char* argv[])
{
    init_glut(argc, argv);
    if (!init_glew())
        std::exit(-1);
    init_GL();

    //------- Scene settings

    // x -> front to back
    // y -> top to bottom
    // z -> left to right
    // scene::Light main_light = scene::Light({0.0, 10.0, 0.0}, 5.f);
    // scene::lights.add_light(main_light);
    const glm::vec3 delta_light{0.f, 0.5f, 0.f};
    // Fire
    const glm::vec3 fire_origin{18.f, 0.75f, 4.f};
    const scene::Light fire_light =
        scene::Light(fire_origin + delta_light, 4.f);
    scene::lights.add_light(fire_light);
    particle::fire_generator = particle::ParticleGenerator(
        glm::vec3(0.f, 1.f, 0.f),          // velocity
        fire_origin,                       // origin
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), // color
        0.1,                               // speed
        1000,   // number of new particles for each update
        10000,  // number of particles
        0.02f); // scale

    // Fire 2
    const glm::vec3 fire_origin2{18.f, 0.75f, -4.f};
    const scene::Light fire_light2 =
        scene::Light(fire_origin2 + delta_light, 4.f);
    scene::lights.add_light(fire_light2);
    particle::fire_generator2 = particle::ParticleGenerator(
        glm::vec3(0.f, 1.f, 0.f),          // velocity
        fire_origin2,                      // origin
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), // color
        0.1,                               // speed
        1000,   // number of new particles for each update
        10000,  // number of particles
        0.02f); // scale

    // Portal A
    const glm::vec3 portalA_origin{3.f, 0.f, 0.f};
    scene::lights.add_light({portalA_origin + delta_light, 0.5f});
    particle::portal_generator_A = particle::ParticleGenerator(
        glm::vec3(0.f, 1.f, 0.f),          // velocity
        portalA_origin,                    // origin
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // color
        0.1,                               // speed
        100,   // number of new particles for each update
        1000,  // number of particles
        0.02f, // scale of particles
        true,  // display in a circle
        false, // color attenuation
        20.f); // base life

    const glm::vec3 portalB_origin{18.f, 0.f, 0.f};
    particle::portal_generator_B = particle::portal_generator_A;
    scene::lights.add_light({portalB_origin + delta_light, 0.5f});
    particle::portal_generator_B.set_position(portalB_origin);

    scene::main_model = scene::Model::from_file("obj/try.obj");

    //------- Init Main shader
    main_shader::init_shaders();
    TEST_OPENGL_ERROR();
    main_shader::init_object_vbo();
    TEST_OPENGL_ERROR();
    main_shader::init_lights();
    main_shader::init_textures();

    //------- Init fire shader
    TEST_OPENGL_ERROR();
    fire_shader::init_shaders();
    TEST_OPENGL_ERROR();
    fire_shader::init_matrices(scene::camera);
    TEST_OPENGL_ERROR();
    fire_shader::init_cube_generator();
    TEST_OPENGL_ERROR();

    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
