#ifndef RENDERING_ATTRIBUTTES_H
#define RENDERING_ATTRIBUTTES_H

#include <list>
#include <cstdint>

enum class RenderingAttribute: uint8_t {DepthTest, CullFace, CullBack, Blend};

using RenderingAttributes = std::list<RenderingAttribute>;

#endif // RENDERING_ATTRIBUTTES_H
