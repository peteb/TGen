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
	
	class PassTextureUnit {
	public:	
		PassTextureUnit(int unit, const std::string & name);
		
		void setTexCoordGen(const std::string & genU, const std::string & genV);
		void setSampler(const std::string & sampler);
		
		
		std::string textureName, samplerName;
		int unit;
		TGen::TextureCoordGen genU, genV;
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
		
		void Update(scalar dt);
		
		void setShader(const std::string & name);
		void AddTextureUnit(PassTextureUnit * textureUnit);
		void Link(TGen::MaterialLinkCallback & callback);
		
	private:
		TGen::BlendFunc StringToBlendFunc(const std::string & blend);
			
		typedef std::vector<TGen::PassTextureUnit *> TextureList;
		TextureList textureUnits;
		
		TGen::RenderContext renderContext;
		TGen::ColorGenerator * colorGen;
		TGen::ScalarGenerator * alphaGen;
		
		std::string shaderName;
	};
	
	class PassList {
	public:
		PassList();
		~PassList();
		
		void Render(TGen::Renderer & renderer, TGen::Renderable & renderable);
		void addPass(TGen::Pass * pass);
		void Link(TGen::MaterialLinkCallback & callback);
		void Update(scalar time);
		
	private:
		typedef std::vector<Pass *> PassVector;
		PassVector passes;
	};
	
} // !TGen

#endif // !_TGEN_PASS_H
