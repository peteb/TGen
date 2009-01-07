/*
 *  passtextureunit.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_PASSTEXTUREUNIT_H
#define _TGEN_GRAPHICS_PASSTEXTUREUNIT_H

#include <string>
#include <tgen_core.h>
#include "renderer_types.h"

namespace TGen {
	class TextureTransformer;
	class TextureUnit;
	class ShaderVariable;
	
	class PassTextureUnit {
	public:	
		PassTextureUnit(int unit, const std::string & name);
		~PassTextureUnit();
		
		void setTextureName(const std::string & name);
		void setTexCoordGen(const std::string & genU, const std::string & genV);
		void setSampler(const std::string & sampler);
		void setWrap(const std::string & wrapU, const std::string & wrapV);
		void addTextureTransformer(TGen::TextureTransformer * transformer);
		void update(scalar time);
		void updateShaderVariables();
		
		std::string textureName, samplerName;
		int unit;
		TGen::TextureCoordGen genU, genV;
		TGen::TextureWrap wrapU, wrapV;
		
		typedef std::vector<TGen::TextureTransformer *> TransformerList;
		TransformerList transformers;
		TGen::TextureUnit * texunit;
		
		typedef std::vector<TGen::ShaderVariable *> ShaderVariableList;
		
		ShaderVariableList binders;
		
		int textureType;
	};	
	
} // !TGen

#endif // !_TGEN_GRAPHICS_PASSTEXTUREUNIT_H
