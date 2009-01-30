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
#include "renderer/forward/renderervars.h"
#include "materialoverride.h"
#include <tgen_math.h>

namespace TGen {
	class Renderer;
	class Material;
	class FrameBuffer;
	class Texture;
	class RenderList;
	class VariableRegister;
	
	class Convex {
	public:
		std::vector<TGen::Vector3> vertices;
		std::vector<TGen::Vector3> normals;
		std::vector<TGen::Vector3> tangents;
		
		void computeBounds(const TGen::Vector3 & dir, float & min, float & max) {
			min = max = TGen::Vector3::DotProduct(vertices[0], dir);
			
			for (int i = 1; i < vertices.size(); ++i) {
				float d = TGen::Vector3::DotProduct(vertices[i], dir);
				min = std::min(min, d);
				max = std::max(max, d);
			}
		}
		
		bool separated(const TGen::Vector3 & dir, const Convex & b) {
			float mina, minb;
			float maxa, maxb;
			
			computeBounds(dir, mina, maxa);
			computeBounds(dir, minb, maxb);
			
			return (maxa + 0.000001f < minb || mina > maxb + 0.000001f);			
		}
		
		bool intersect(const Convex & b) {
			for (int i = 0; i < normals.size(); ++i) {
				if (separated(normals[i], b))
					return false;
			}
			
			for (int i = 0; i < b.normals.size(); ++i) {
				if (separated(b.normals[i], b))
					return false;
			}
			
			for (int i = 0; i < tangents.size(); ++i) {
				for (int j = 0; j < b.tangents.size(); ++j) {
					if (separated(TGen::Vector3::CrossProduct(tangents[i], b.tangents[j]), b))
						return false;
				}
			}
			
			return true;
		}
	};
	
	
	
	// ^^^^^^^^ generic code actually, should be moved if it's used
	
	
	namespace Engine {
		class World;
		class ResourceManager;
		class Light;
		
		enum RenderPass {
			DepthPass,
			LightPass,
			ShadowPass,
			AmbientPass,
		};
		
		class ForwardRenderer : public TGen::Engine::WorldRenderer, public TGen::MaterialOverride {
		public:
			ForwardRenderer(TGen::Renderer & renderer, TGen::Engine::ResourceManager & resources, TGen::Engine::VariableRegister & variables);
			~ForwardRenderer();
			
			void renderWorld(TGen::Engine::World & world, TGen::Camera * camera, scalar dt);
			void overrideMaterial(TGen::Renderer & renderer, int param) const;
			
		private:
			void renderDepth(TGen::RenderList & renderList, TGen::Camera * camera);
			void renderShadowmap(TGen::RenderList & renderList, TGen::Engine::Light * light, const TGen::Matrix4x4 & transform);
			void renderShadowedLight(TGen::Engine::Light * light, const TGen::Matrix4x4 & transform, TGen::RenderList & renderList, TGen::Camera * camera);
			void renderLight(TGen::Engine::Light * light, TGen::RenderList & renderList, TGen::Camera * camera);
			
			static TGen::Matrix4x4 calculateLightProjection(const TGen::Engine::Light & light);
			static TGen::Matrix4x4 calculateLightModelView(const TGen::Matrix4x4 & lightTransform);
			static bool calculateFrustumBox(TGen::Rectangle & outRectangle, const TGen::Matrix4x4 & frustumTransform, const TGen::Matrix4x4 & cameraProj, const TGen::Matrix4x4 & cameraTransform);
			
			static TGen::Frustum calculateFrustum(const TGen::Matrix4x4 & transform);
			static TGen::Convex calculateFrustumConvex(const TGen::Matrix4x4 & transform);
			
			TGen::FrameBuffer * shadowMapTarget;
			TGen::Texture * shadowMap, * crapMap, * lightMap;
			TGen::Material * depthPassMaterial, * currentLightMaterial;
			TGen::Matrix4x4 shadowMatrix, shadowFrustumMat;
			RenderPass currentPass;
			TGen::Frustum shadowFrustum;
			
			TGen::Engine::ForwardRendererVars vars;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FORWARDRENDERER_RENDERER_H

