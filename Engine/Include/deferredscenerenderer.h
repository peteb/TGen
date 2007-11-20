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
#include <tgen_math.h>
#include "deferredrenderervars.h"

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
		
		class DeferredRenderer {
		public:	
			DeferredRenderer(TGen::Engine::App & app, TGen::Engine::World & world);
			~DeferredRenderer();
			
			void renderScene(scalar dt);
			void postProcessing(const TGen::Rectangle & viewport);
			
		private:
			void renderFillQuad(TGen::Material * material);
			void renderPostFillQuad(TGen::Material * material);
			
			void createResources(const TGen::Rectangle & mapSize);
			int ceilPowerOfTwo(int value);
			
			TGen::Engine::App & app;
			TGen::Engine::World & world;
			TGen::Engine::DeferredRendererVars vars;

			TGen::Rectangle mrtSize;
			TGen::Camera * mainCamera;
			
			// resources
			TGen::ShaderProgram * rhwNoTransformShader;
			TGen::Mesh * screenFillMesh;
			TGen::Material * lightAmbientMaterial, * lightDirectionalMaterial;
			TGen::Texture * colorMap, * depthMap, * normalMap, * miscMap, * postMap1;
			TGen::FrameBuffer * mapTargets, * postTargets1;
		};
	}
}

#endif // !_TGEN_ENGINE_DEFERREDSCENERENDERER_H
