/*
 *  worldrenderer.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/21/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_RENDERER_WORLDRENDERER_H
#define _TGEN_ENGINE_RENDERER_WORLDRENDERER_H

#include <tgen_core.h>

namespace TGen {
	class Renderer;
	
	namespace Engine {
		class World;
		
		class WorldRenderer {
		public:	
			WorldRenderer(TGen::Renderer & renderer);
			virtual ~WorldRenderer();
			
			virtual void renderWorld(TGen::Engine::World & world, scalar dt) abstract;
			TGen::Renderer & getRenderer();
			
		protected:
			TGen::Renderer & renderer;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_RENDERER_WORLDRENDERER_H

