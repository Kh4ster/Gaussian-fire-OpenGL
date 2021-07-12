#pragma once

#include "shader_handler.hh"
#include "program_propreties.hh"

namespace quad_shader
{

bool init_shaders()
{
    return shader::init_shaders("shaders/debug_quad_vertex.shd",
                                "shaders/debug_quad_fragment.shd",
                                quad_program_id);
}
} // namespace quad_shader
