/*
 *  passlist.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 8/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_PASSLIST_H
#define _TGEN_PASSLIST_H

#include <tgen_math.h>
#include <vector>

namespace TGen {
	class Pass;
	class Renderer;
	class MaterialLinkCallback;
	class Texture;
	class Renderable;
	
	class PassList {
	public:
		PassList();
		~PassList();
		
		void render(TGen::Renderer & renderer, const TGen::Renderable & renderable, TGen::Texture ** textureTypes);
		void addPass(TGen::Pass * pass);
		void link(TGen::MaterialLinkCallback & callback);
		void update(scalar time);
		
	private:
		typedef std::vector<Pass *> PassVector;
		PassVector passes;
	};	
	
} // !TGen

#endif // !_TGEN_PASSLIST_H
