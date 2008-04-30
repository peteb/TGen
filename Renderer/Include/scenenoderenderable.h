/*
 *  scenenoderenderable.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 4/4/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_SCENENODERENDERABLE_H
#define _TGEN_RENDERER_SCENENODERENDERABLE_H

#include <tgen_graphics.h>

namespace TGen {
	class SceneNode;
	class Renderer;
	
	class SceneNodeRenderable : public TGen::Renderable {
	public:
		SceneNodeRenderable(TGen::SceneNode & node);
		
		void preRender(TGen::Renderer & renderer) const;
		void render(TGen::Renderer & renderer) const;
		
	private:
		TGen::SceneNode & node;
	};
} // !TGen

#endif // !_TGEN_RENDERER_SCENENODERENDERABLE_H

