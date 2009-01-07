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
	
	class PassTextureUnit;
	class ShaderMode;


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
		typedef std::map<int, ShaderMode *> ShaderModeMap;
		
		
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
