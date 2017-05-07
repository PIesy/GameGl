#ifndef RENDERING_ATTRIBUTTES_H
#define RENDERING_ATTRIBUTTES_H

#include <vector>
#include <cstdint>

enum class RenderingAttribute: uint8_t {DepthTest, CullFace, CullBack, Blend, StencilTest, ClearBuffer, UpdateMipmaps};

using RenderingAttributes = std::vector<RenderingAttribute>;

const RenderingAttributes defaultAttributes = {RenderingAttribute::DepthTest, RenderingAttribute::Blend, RenderingAttribute::ClearBuffer};

#endif // RENDERING_ATTRIBUTTES_H
