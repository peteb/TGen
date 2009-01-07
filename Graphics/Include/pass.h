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
		
		void setColor(const TGen::Color & color);
		void setAlpha(scalar a);
		void setDepthFunc(TGen::CompareFunc func);
		void setFrontMode(TGen::PolygonFaceMode mode);
		void setBackMode(TGen::PolygonFaceMode mode);
		void setBlendFunc(TGen::BlendFunc src, TGen::BlendFunc dest);
		void setColorGenerator(TGen::ColorGenerator * gen);
		void setAlphaGenerator(TGen::ScalarGenerator * gen);
		void setNoDepthWrite();
		void setDisableDepth();
		void setDisableBlend();
		void setColorFromVertex();
		void setLightDiffuse(const TGen::Color & diffuse);
		void setLightSpecular(const TGen::Color & specular);
		void setLightShininess(scalar shininess);

		void link(TGen::MaterialLinkCallback & callback);
		void update(scalar dt);
		void updateVariables(TGen::ShaderVariableUpdater * varupdater);
	
		void addShaderVariable(const std::string & varname, const std::string & linkdid);
		void addShaderUpdater(TGen::ShaderUpdater * updater);
		void addTextureUnit(PassTextureUnit * textureUnit);
		
		void setShader(const std::string & name, int mode);
		int getNumTextureUnits() const;
		
	private:			
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
