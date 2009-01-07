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
		void setLinkedUnit(TGen::TextureUnit * texunit);
		
		void addTextureTransformer(TGen::TextureTransformer * transformer);
		void addTransformBinder(TGen::ShaderVariable * var);
		
		void update(scalar time);
		void updateShaderVariables();
		const std::string & getTextureName() const;
		const std::string & getSamplerName() const;
		
		int getUnit() const;

		TGen::TextureCoordGen genU, genV;
		TGen::TextureWrap wrapU, wrapV;

	private:
		std::string textureName, samplerName;
		
		typedef std::vector<TGen::TextureTransformer *> TransformerList;
		typedef std::vector<TGen::ShaderVariable *> ShaderVariableList;

		TransformerList transformers;
		ShaderVariableList binders;

		TGen::TextureUnit * texunit;
		
		int textureType, unit;
	};	
	
} // !TGen

#endif // !_TGEN_GRAPHICS_PASSTEXTUREUNIT_H
