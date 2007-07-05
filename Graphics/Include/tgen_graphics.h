/*
 *  tgen_graphics.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/2/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_H
#define _TGEN_GRAPHICS_H

#include "tgen_core.h"
#include "tgen_math.h"

#include "renderer.h"
#include "renderer_types.h"
#include "renderer_caps.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "texture.h"
#include "vertexstructure.h"
#include "shader.h"
#include "shaderprogram.h"
#include "shadervariable.h"
#include "framebuffer.h"
#include "rendercontext.h"

#include "image.h"
#include "canvas.h"
#include "color.h"

#include "batch.h"
#include "material.h"
#include "materialparser.h"
#include "technique.h"
#include "pass.h"
#include "renderable.h"

namespace TGen {
	bool isGraphicsDebug();
}

#endif // !_TGEN_GRAPHICS_H
