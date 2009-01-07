/*
 *  pass.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 7/4/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_PASS_H
#define _TGEN_PASS_H

#include <vector>
#include "rendercontext.h"
#include "renderer_types.h"

namespace TGen {
	class Renderable;
	class Renderer;
	class MaterialLinkCallback;
	class PassShaderVariable;
	class ShaderVariableUpdater;
	class ShaderUpdater;
	class ShaderVariable;
	class TextureTransformer;
	class ColorGenerator;
	class ScalarGenerator;
	
	
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
	
	class ShaderMode {
	public:
		ShaderMode(const std::string & name)
			: name(name)
			, shader(NULL)
		{
		}

		void link(TGen::MaterialLinkCallback & callback);
		void update();
		void addShaderUpdater(TGen::ShaderUpdater * updater);
		
		typedef std::vector<TGen::ShaderUpdater *> ShaderUpdaterList;

		
		ShaderUpdaterList shaderUpdaters;
		std::string name;
		TGen::ShaderProgram * shader;
	};
	
	class Pass {
	public:	
		Pass();
		~Pass();
		
		const TGen::RenderContext & getRenderContext(int shaderMode);
		
		void setColor(const std::string & r, const std::string & g, const std::string & b);
		void setAlpha(const std::string & a);
		void setDepthFunc(const std::string & func);
		void setFrontMode(const std::string & mode);
		void setBackMode(const std::string & mode);
		void setBlendFunc(const std::string & source, const std::string & dest);
		void setColorGenerator(TGen::ColorGenerator * gen);
		void setAlphaGenerator(TGen::ScalarGenerator * gen);
		void setNoDepthWrite();
		void setDisableDepth();
		void setDisableBlend();
		void setColorFromVertex();
		void setLightDiffuse(const TGen::Color & diffuse);
		void setLightSpecular(const TGen::Color & specular);
		void setLightShininess(scalar shininess);
		
		void update(scalar dt);
		
		void updateVariables(TGen::ShaderVariableUpdater * varupdater);
		void addShaderVariable(const std::string & varname, const std::string & linkdid);
		void addShaderUpdater(TGen::ShaderUpdater * updater);
		
		void setShader(const std::string & name, int mode);
		void addTextureUnit(PassTextureUnit * textureUnit);
		int getNumTextureUnits() const;
		void link(TGen::MaterialLinkCallback & callback);
		
	private:
		static TGen::BlendFunc StringToBlendFunc(const std::string & blend);
			
		typedef std::vector<TGen::PassTextureUnit *> TextureList;
		typedef std::vector<TGen::PassShaderVariable *> ShaderVarList;
		typedef std::map<int, ShaderMode> ShaderModeMap;
		
		
		ShaderModeMap shaderModes;
		ShaderVarList shaderVariables;
		TextureList textureUnits;
		
		TGen::RenderContext renderContext;
		TGen::ColorGenerator * colorGen;
		TGen::ScalarGenerator * alphaGen;
		
		std::string shaderName;
	};
	

	
} // !TGen

#endif // !_TGEN_PASS_H
