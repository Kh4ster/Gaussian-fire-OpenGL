//************************************************
//*                                              *
//*   TP 1&2    (c) 2017 J. FABRIZIO             *
//*                                              *
//*                               LRDE EPITA     *
//*                                              *
//************************************************

#pragma once

#include "image.hh"

namespace tifo
{

bool save_image(rgb24_image& image, const char* filename);
rgb24_image* load_image(const char* filename);

} // namespace tifo