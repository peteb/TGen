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
#include "deferredrenderervars.h"

namespace TGen {	
	class ShaderProgram;
	class Mesh;
	class Material;
	class Texture;
	class FrameBuffer;
	class Rectangle;
	class Camera;
	class ShaderVariable;
	
	namespace Engine {
		class App;
		class World;

		class DeferredRenderer : public TGen::ShaderVariableUpdater {
		public:	
			DeferredRenderer(TGen::Engine::App & app);
			~DeferredRenderer();
			
			void renderScene(scalar dt);
			void postProcessing(const TGen::Rectangle & viewport);
			void updateShaderVariable(TGen::ShaderVariable & var, const std::string & name);
			void setWorld(TGen::Engine::World * world);
			
		private:
			void renderWorldless(scalar dt);
			
			void renderFillQuad(TGen::Material * material);
			void renderFillQuad(TGen::Material * material, const std::string & specialization);

			void renderPostFillQuad(TGen::Material * material);
			void renderPost2FillQuad(TGen::Material * material);
			void renderPost3FillQuad(TGen::Material * material);
			void renderPostFinalQuad(TGen::Material * material);
			
			void createResources(const TGen::Rectangle & mapSize);
			void loadLightMaterial(const std::string & name, int materialId);
			int ceilPowerOfTwo(int value);
			
			TGen::Engine::App & app;
			TGen::Engine::World * world;
			TGen::Engine::DeferredRendererVars vars;

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
