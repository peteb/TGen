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
		void update(scalar time);
		
		std::string textureName, samplerName;
		int unit;
		TGen::TextureCoordGen genU, genV;
		TGen::TextureWrap wrapU, wrapV;

		typedef std::vector<TGen::TextureCoordTransformer *> TransformerList;
		TransformerList transformers;
		TGen::TextureUnit * texunit;
		int textureType;
	};
	
	
	class Pass {
	public:	
		Pass();
		~Pass();
		
		const TGen::RenderContext & getRenderContext() const;
		
		void setColor(const std::string & r, const std::string & g, const std::string & b);
		void setAlpha(const std::string & a);
		void setDepthFunc(const std::string & func);
		void setFrontMode(const std::string & mode);
		void setBackMode(const std::string & mode);
		void setBlendFunc(const std::string & source, const std::string & dest);
		void setColorGenerator(TGen::ColorGenerator * gen);
		void setAlphaGenerator(TGen::ScalarGenerator * gen);
		void setNoDepthWrite();
		void setColorFromVertex();
		
		void update(scalar dt);
		
		void updateVariables(TGen::ShaderVariableUpdater * varupdater);
		void addShaderVariable(const std::string & varname, const std::string & linkdid);
		void setShader(const std::string & name);
		void addTextureUnit(PassTextureUnit * textureUnit);
		void link(TGen::MaterialLinkCallback & callback);
		
	private:
		static TGen::BlendFunc StringToBlendFunc(const std::string & blend);
			
		typedef std::vector<TGen::PassTextureUnit *> TextureList;
		typedef std::vector<TGen::PassShaderVariable *> ShaderVarList;
		
		ShaderVarList shaderVariables;
		TextureList textureUnits;
		
		TGen::RenderContext renderContext;
		TGen::ColorGenerator * colorGen;
		TGen::ScalarGenerator * alphaGen;
		
		std::string shaderName;
	};
	

	
} // !TGen

#endif // !_TGEN_PASS_H
