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

namespace TGen {
	class ShaderProgram;
	class Texture;
	
	class TextureUnit {
	public:
		TextureUnit(int unit, TGen::Texture * texture);
		
		int unit;
		TGen::Texture * texture;
		TGen::TextureCoordGen genU, genV;
		TGen::Matrix4x4 transform;
		bool transformed;
	};
	
	class RenderContext {
	public:
		RenderContext();
		
		void AddTextureUnit(TGen::TextureUnit * unit);
		
		bool depthWrite;
		TGen::Color frontColor;
		TGen::ShaderProgram * shader;
		TGen::CompareFunc depthFunc;
		TGen::PolygonFaceMode front, back;
		TGen::BlendFunc blendSrc, blendDst;
		
		typedef std::vector<TGen::TextureUnit *> TextureList;
		TextureList textureUnits;
	};
	
} // !TGen

#endif // !_TGEN_RENDERCONTEXT_H
