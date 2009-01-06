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
	class ColorGenerator;
	class ScalarGenerator;
	class PassShaderVariable;
	class ShaderVariableUpdater;
	class ShaderUpdater;
	class ShaderVariable;
	
	class TextureCoordTransformer {
	public:
		TextureCoordTransformer(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool centered);
		virtual ~TextureCoordTransformer();
		
		virtual void applyTransform(TGen::Matrix4x4 & matrix, scalar time) abstract;

		bool centered;

	protected:
		scalar startedAt;
		TGen::ScalarGenerator * genU, * genV;
	};
	
	
	class TextureCoordTranslate : public TGen::TextureCoordTransformer {
	public:
		TextureCoordTranslate(float u, float v, bool scroll = false);
		TextureCoordTranslate(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool scroll = false);
		
		void applyTransform(TGen::Matrix4x4 & matrix, scalar time);

		float u, v;
		bool scroll;
	};

	class TextureCoordScale : public TGen::TextureCoordTransformer {
	public:
		TextureCoordScale(float u, float v, bool centered = false);
		TextureCoordScale(TGen::ScalarGenerator * genU, TGen::ScalarGenerator * genV, bool centered = false);
		
		void applyTransform(TGen::Matrix4x4 & matrix, scalar time);
		
		float u, v;
	};	
	
	class TextureCoordRotate : public TGen::TextureCoordTransformer {
	public:
		TextureCoordRotate(float speed, bool centered = false);
		TextureCoordRotate(TGen::ScalarGenerator * genRot, bool centered = false);
		
		void applyTransform(TGen::Matrix4x4 & matrix, scalar time);
		
		float speed;
	};
	
	class PassTextureUnit {
	public:	
		PassTextureUnit(int unit, const std::string & name);
		~PassTextureUnit();
		
		void setTextureName(const std::string & name);
		void setTexCoordGen(const std::string & genU, const std::string & genV);
		void setSampler(const std::string & sampler);
		void setWrap(const std::string & wrapU, const std::string & wrapV);
		void addTexCoordTransformer(TGen::TextureCoordTransformer * transformer);
		void update(scalar time, bool update = true);
		
		std::string textureName, samplerName;
		int unit;
		TGen::TextureCoordGen genU, genV;
		TGen::TextureWrap wrapU, wrapV;

		typedef std::vector<TGen::TextureCoordTransformer *> TransformerList;
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
