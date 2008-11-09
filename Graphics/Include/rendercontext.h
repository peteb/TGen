/*
 *  rendercontext.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERCONTEXT_H
#define _TGEN_RENDERCONTEXT_H

#include "color.h"
#include "renderer_types.h"
#include <vector>
#include "tgen_math.h"
#include "lightmaterial.h"

namespace TGen {
	class ShaderProgram;
	class Texture;
	class TextureUnit;

	
	class RenderContext {
	public:
		RenderContext();
		
		void addTextureUnit(TGen::TextureUnit * unit);
		
		bool depthWrite, disableDepth, disableBlend;
		TGen::Color frontColor;
		TGen::ShaderProgram * shader;
		TGen::CompareFunc depthFunc;
		TGen::PolygonFaceMode front, back;
		TGen::BlendFunc blendSrc, blendDst;
		bool colorFromVertex;
		
		typedef std::vector<TGen::TextureUnit *> TextureList;
		TextureList textureUnits;
		LightMaterial lightMaterial;
	};
	
} // !TGen

#endif // !_TGEN_RENDERCONTEXT_H
