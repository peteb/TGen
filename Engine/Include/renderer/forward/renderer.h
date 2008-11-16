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
#include "materialoverride.h"
#include <tgen_math.h>

namespace TGen {
	class Renderer;
	class Material;
	class FrameBuffer;
	class Texture;
	
	namespace Engine {
		class World;
		class ResourceManager;
		class Light;
		
		enum RenderPass {
			DepthPass,
			LightPass,
			ShadowPass,
		};
		
		class ForwardRenderer : public TGen::Engine::WorldRenderer, public TGen::MaterialOverride {
		public:
			ForwardRenderer(TGen::Renderer & renderer, TGen::Engine::ResourceManager & resources);
			~ForwardRenderer();
			
			void renderWorld(TGen::Engine::World & world, TGen::Camera * camera, scalar dt);
			void overrideMaterial(TGen::Renderer & renderer, int param) const;
			
		private:
			TGen::FrameBuffer * shadowMapTarget;
			TGen::Texture * shadowMap, * crapMap;
			TGen::Material * depthPassMaterial;
			TGen::Matrix4x4 shadowMatrix;
			RenderPass currentPass;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FORWARDRENDERER_RENDERER_H

