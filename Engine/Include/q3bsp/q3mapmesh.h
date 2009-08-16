/*
 *  q3mapmesh.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/27/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_Q3MAPMESH_H
#define _TGEN_ENGINE_Q3MAPMESH_H

#include <tgen_renderer.h>

namespace TGen {
	namespace Engine {
		class Q3MapMesh : public TGen::NewMesh {
		public:
			Q3MapMesh(const std::string & material);
			
			typedef TGen::Vertex3<float> Vertex;
			typedef TGen::TexCoord2<float, 0> TexCoord;
			typedef TGen::TexCoord2<float, 1> LightCoord;
			typedef TGen::Color3<float> Color;
			
			typedef TGen::JoinVertexElements4<Vertex, TexCoord, LightCoord, Color> VertexDecl;
			
			TGen::NewMeshInstance * instantiate(TGen::VertexDataSource & source);
			void render(TGen::Renderer & renderer) const;
			void preRender(TGen::Renderer & renderer) const;
			
			void addVertex(const VertexDecl::Type & vertex);
			
		private:
			std::vector<VertexDecl::Type> vertices;
			
			TGen::VertexData * vb, * ib;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_Q3MAPMESH_H

