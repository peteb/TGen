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
#include <tgen_graphics.h> 
#include "renderer/deferred/renderervars.h"
#include "renderer/worldrenderer.h"

namespace TGen {	
	class ShaderProgram;
	class Mesh;
	class Material;
	class Texture;
	class FrameBuffer;
	class Rectangle;
	class Camera;
	class ShaderVariable;
	class Renderer;
	
	namespace Engine {
		struct StandardLogs;
		class VariableRegister;
		class World;
		class ResourceManager;
		
		class DeferredRenderer : public TGen::Engine::WorldRenderer, TGen::ShaderVariableUpdater {
		public:	
			DeferredRenderer(TGen::Renderer & renderer, TGen::Engine::StandardLogs & logs, TGen::Engine::VariableRegister & variables, TGen::Engine::ResourceManager & resources);
			~DeferredRenderer();
			
			void renderWorld(TGen::Engine::World & world, scalar dt);
			void postProcessing(const TGen::Rectangle & viewport);
			void updateShaderVariable(TGen::ShaderVariable & var, const std::string & name);
			//void setWorld(TGen::Engine::World * world);
			
		private:
			void renderWorldless(scalar dt);
			
			void renderFillQuad(TGen::Material * material);
			void renderFillQuad(TGen::Material * material, const std::string & specialization);

			void renderPostFillQuad(TGen::Material * material);
			void renderPost2FillQuad(TGen::Material * material);
			void renderPost3FillQuad(TGen::Material * material);
			void renderPostFinalQuad(TGen::Material * material);
			
			void createResources(const TGen::Rectangle & mapSize);
			void loadLightMaterial(const std::string & name, int materialId, TGen::Engine::ResourceManager & resources);
			int ceilPowerOfTwo(int value);
			
			//TGen::Engine::App & app;
		//	TGen::Renderer & renderer;
			TGen::Engine::StandardLogs & logs;
			TGen::Engine::VariableRegister & variables;
			TGen::Engine::ResourceManager & resources;

			TGen::Engine::DeferredRendererVars vars;

		//	TGen::Engine::World * world;	// is linked

			TGen::Rectangle mrtSize, downsampleSize;
			TGen::Camera * mainCamera;
			int lastNumLights;
			int lightBatchSize;
			
			// resources
			TGen::ShaderProgram * rhwNoTransformShader;
			TGen::Mesh * screenFillMesh;
			TGen::Material * lightAmbientMaterial, * lightDirectionalMaterial, * lightPositionalMaterial, * postLuminanceMaterial, * postGaussianHorizMaterial, * postGaussianVertMaterial, * postFinalBloom, * rhwOnlyColorMaterial;
			TGen::Material * metaNormalMaterial;
			TGen::Texture * colorMap, * depthMap, * normalMap, * miscMap, * postMap1, * postMap2, * postMap3;
			TGen::FrameBuffer * mapTargets, * postTargets1, * postTargets2, * postTargets3;
			TGen::FrameBuffer * depthTarget;
			TGen::Material ** lightMaterials;
			
			typedef TGen::Vertex3<float> LightVertexDecl;
			typedef TGen::JoinVertexElements2<TGen::Vertex3<float>, TGen::Color4<float> > MetaLineDecl;
			
			TGen::Batch<MetaLineDecl, 2> metaLines;
		};
	}
}

#endif // !_TGEN_ENGINE_DEFERREDSCENERENDERER_H
