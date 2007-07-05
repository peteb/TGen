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
	
	class Pass {
	public:	
		Pass();
		~Pass();
		
		const TGen::RenderContext & getRenderContext() const;
		
		void setColor(const std::string & r, const std::string & g, const std::string & b);
		void setShader(const std::string & name);
		void Link(TGen::MaterialLinkCallback & callback);
		
	private:
		TGen::RenderContext renderContext;
		std::string shaderName;
	};
	
	class PassList {
	public:
		PassList();
		~PassList();
		
		void Render(TGen::Renderer & renderer, TGen::Renderable & renderable);
		void addPass(TGen::Pass * pass);
		void Link(TGen::MaterialLinkCallback & callback);
		
	private:
		typedef std::vector<Pass *> PassVector;
		PassVector passes;
	};
	
} // !TGen

#endif // !_TGEN_PASS_H
