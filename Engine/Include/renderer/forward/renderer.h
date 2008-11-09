/*
 *  renderer.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/6/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_FORWARDRENDERER_RENDERER_H
#define _TGEN_ENGINE_FORWARDRENDERER_RENDERER_H

#include "renderer/worldrenderer.h"

namespace TGen {
	class Renderer;
	
	namespace Engine {
		class World;
		
		class ForwardRenderer : public TGen::Engine::WorldRenderer {
		public:
			ForwardRenderer(TGen::Renderer & renderer);
			~ForwardRenderer();
			
			void renderWorld(TGen::Engine::World & world, TGen::Camera * camera, scalar dt);
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FORWARDRENDERER_RENDERER_H

