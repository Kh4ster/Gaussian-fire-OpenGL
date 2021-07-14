#pragma once

#include "shader_handler.hh"
#include "program_propreties.hh"
#include "camera.hh"
#include "image.hh"

namespace fire_shader
{

bool init_shaders()
{
    return shader::init_shaders("shaders/fire_vertex.shd",
                                "shaders/fire_fragment.shd",
                                fire_program_id);
}

void init_matrices(scene::Camera& camera)
{
    camera.update_camera(fire_program_id);
}

} // namespace fire_shader