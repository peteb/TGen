/*
 *  cubemesh.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_CUBEMESH_H
#define _TGEN_ENGINE_CUBEMESH_H

#include <tgen_renderer.h>

namespace TGen {
	class Renderer;
	
	namespace Engine {
		class CubeMesh : public TGen::NewMesh {
		public:
			CubeMesh(const std::string & materialName, const std::string & materialNamePostfix, const TGen::Vector3 & min, const TGen::Vector3 & max, const TGen::Vector3 & texMin, const TGen::Vector3 & texMax);
			~CubeMesh();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			void updateShaderVariable(TGen::ShaderVariable & var, const std::string & name);

			void createVertexData(TGen::VertexDataSource & dataSource);
			TGen::Engine::CubeMesh * instantiate(TGen::VertexDataSource & source) const;
			
		private:
			typedef TGen::TexCoord2<float, 0> TexCoordDecl;
			typedef TGen::Vertex3<float> VertexCoordDecl;
			typedef TGen::Normal3<float> NormalDecl;
			typedef TGen::VertexAttribute4<10, float> Tangent;

			typedef TGen::JoinVertexElements4<VertexCoordDecl, NormalDecl, TexCoordDecl, Tangent> VertexDecl;
			
			TGen::Vector3 min, max, texMin, texMax;
			TGen::VertexData * vb;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CUBEMESH_H

