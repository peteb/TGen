/*
 *  printscene.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// A scene graph walker that outputs the tree in text.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_PRINTSCENE_H
#define _TGEN_RENDERER_PRINTSCENE_H

#include "scenenode.h"

namespace TGen {
	class ScenePrinter : public TGen::SceneNode::Walker {
	public:
		ScenePrinter(std::ostream & stream);
		
		bool pre(TGen::SceneNode & node) const;
		void post(TGen::SceneNode & node) const;
		
	private:
		std::ostream & stream;
		mutable int level;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_PRINTSCENE_H