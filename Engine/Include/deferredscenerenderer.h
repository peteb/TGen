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
	class Texture;
	class FrameBuffer;
	class Rectangle;
	class Camera;
	
	namespace Engine {
		class App;
		class World;
		
		class DeferredSceneRenderer {
		public:	
			DeferredSceneRenderer(TGen::Engine::App & renderer, TGen::Engine::World & world);
			~DeferredSceneRenderer();
			
			void renderScene();
			
		private:
			void renderFillQuad();
			
			void createResources(const TGen::Rectangle & mapSize);
			int ceilPowerOfTwo(int value);
			
			TGen::Engine::App & app;
			TGen::Engine::World & world;
			
			TGen::Camera * mainCamera;
			
			// resources
			TGen::ShaderProgram * rhwNoTransformShader;
			TGen::Mesh * screenFillMesh;
			TGen::Material * screenFillMaterial;
			TGen::Texture * colorMap, * depthMap, * normalMap;
			TGen::FrameBuffer * mapTargets;
		};
	}
}

#endif // !_TGEN_ENGINE_DEFERREDSCENERENDERER_H
