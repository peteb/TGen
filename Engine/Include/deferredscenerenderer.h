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

#include <string>

namespace TGen {	
	class ShaderProgram;
	class Mesh;
	class Material;
	
	namespace Engine {
		class App;
		
		class DeferredSceneRenderer {
		public:	
			DeferredSceneRenderer(TGen::Engine::App & renderer);
			~DeferredSceneRenderer();
			
			void renderScene();
			
		private:
			void renderFillQuad();
			
			TGen::Engine::App & app;
			
			
			
			// resources
			TGen::ShaderProgram * rhwNoTransformShader;
			TGen::Mesh * screenFillMesh;
			TGen::Material * screenFillMaterial;
		};
	}
}

#endif // !_TGEN_ENGINE_DEFERREDSCENERENDERER_H
