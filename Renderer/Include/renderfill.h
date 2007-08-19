/*
 *  renderfill.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// The RenderFiller is a scene graph walker that asks ech node to add it's faces to the list.
// A class inherited from the SceneNode class can override this method to provide own functionality,
// like a space partition traverser.
//
// see also: SceneNode::Walker, scenenode.h
// keywords: walker
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_RENDERFILL_H
#define _TGEN_RENDERER_RENDERFILL_H

#include "scenenode.h"

namespace TGen {
	class RenderList;
	class Camera;
	
	class RenderFiller : public TGen::SceneNode::Walker {
	public:	
		RenderFiller(TGen::RenderList & list, const TGen::Camera & camera);
		
		bool pre(TGen::SceneNode & node) const;
		void post(TGen::SceneNode & node) const;
		
	private:
		TGen::RenderList & list;
		const TGen::Camera & camera;
	};
	
	
} // !TGen

#endif // !_TGEN_RENDERER_RENDERFILL_H
