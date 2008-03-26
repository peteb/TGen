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
			CubeMesh(const std::string & materialName, const std::string & materialNamePostfix, const TGen::Vector3 & min, const TGen::Vector3 & max);
			~CubeMesh();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			
			void createVertexData(TGen::VertexDataSource & dataSource);
			TGen::Engine::CubeMesh * instantiate() const;
			
		private:
			typedef TGen::TexCoord2<float, 0> TexCoordDecl;
			typedef TGen::Vertex3<float> VertexCoordDecl;
			typedef TGen::Normal3<float> NormalDecl;
			typedef TGen::VertexAttribute3<40, float> SecondVertexCoordDecl;
			typedef TGen::VertexAttribute3<41, float> SecondNormalDecl;
			
			typedef TGen::JoinVertexElements3<VertexCoordDecl, NormalDecl, TexCoordDecl>			VertexDecl;
			typedef TGen::JoinVertexElements2<VertexCoordDecl, NormalDecl >							FrameVertexDecl;	
			typedef TGen::JoinVertexElements5<TexCoordDecl, VertexCoordDecl, NormalDecl, SecondVertexCoordDecl, SecondNormalDecl> DoubleVertexDecl;
			
			TGen::Vector3 min, max;
			TGen::VertexData * vb;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CUBEMESH_H

