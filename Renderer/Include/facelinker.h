/*
 *  facelinker.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// A scene graph walker that links faces to materials in the tree.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_FACELINKER_H
#define _TGEN_RENDERER_FACELINKER_H

#include "scenenode.h"

namespace TGen {
	class MaterialSource;
	
	class FaceLinker : public TGen::SceneNode::Walker {
	public:	
		FaceLinker(TGen::MaterialSource & source);
		
		bool pre(TGen::SceneNode & node) const;
		void post(TGen::SceneNode & node) const;
		
	private:
		TGen::MaterialSource & source;
	};
	
	
} // !TGen

#endif // !_TGEN_RENDERER_FACELINKER_H
