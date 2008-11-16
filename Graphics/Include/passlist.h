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
	class ShaderVariableUpdater;
	class MaterialOverride;
	
	class PassList {	// aka "LOD"
	public:
		PassList();
		~PassList();
		
		void render(TGen::Renderer & renderer, const TGen::Renderable & renderable, TGen::Texture ** textureTypes, TGen::ShaderVariableUpdater * varupdater = NULL, TGen::MaterialOverride * override = NULL);
		void addPass(TGen::Pass * pass);
		void link(TGen::MaterialLinkCallback & callback);
		void update(scalar time);
		void setSortLevel(int level);
		int getSortLevel() const;
	
	private:
		typedef std::vector<Pass *> PassVector;
		PassVector passes;
		
		int sortLevel;
	};	
	
} // !TGen

#endif // !_TGEN_PASSLIST_H
