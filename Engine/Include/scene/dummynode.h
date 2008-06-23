/*
 *  dummynode.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/22/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCENE_DUMMYNODE_H
#define _TGEN_ENGINE_SCENE_DUMMYNODE_H

#include <tgen_renderer.h>

namespace TGen {
	namespace Engine {
		namespace Scene {
			class DummyNode : public TGen::SceneNode {
			public:
				DummyNode(const std::string & name)
					: TGen::SceneNode(name + "_dummy") {}
				
				bool fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const {return false; }
				
			};
			
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_DUMMYNODE_H

