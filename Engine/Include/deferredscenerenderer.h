/*
 *  deferredscenerenderer.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/31/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_DEFERREDSCENERENDERER_H
#define _TGEN_ENGINE_DEFERREDSCENERENDERER_H

namespace TGen {
	class Renderer;
	
	namespace Engine {
		class DeferredSceneRenderer {
		public:	
			DeferredSceneRenderer(TGen::Renderer & renderer);
			~DeferredSceneRenderer();
			
			void renderScene();
			
		private:
			TGen::Renderer & renderer;
		};
	}
}

#endif // !_TGEN_ENGINE_DEFERREDSCENERENDERER_H
