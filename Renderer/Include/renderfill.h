/*
 *  renderfill.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_RENDERFILL_H
#define _TGEN_RENDERER_RENDERFILL_H

#include "scenenode.h"

namespace TGen {
	class RenderList;
	
	class RenderFiller : public TGen::SceneNode::Walker {
	public:	
		RenderFiller(TGen::RenderList & list);
		
		bool pre(TGen::SceneNode & node) const;
		void post(TGen::SceneNode & node) const;
		
	private:
		TGen::RenderList & list;
	};
	
	
} // !TGen

#endif // !_TGEN_RENDERER_RENDERFILL_H
