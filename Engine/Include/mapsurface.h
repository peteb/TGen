/*
 *  mapsurface.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MAPSURFACE_H
#define _TGEN_ENGINE_MAPSURFACE_H

#include <tgen_renderer.h>

namespace TGen {
	namespace Engine {
		enum {
			MapSurfaceTangentAttribute = 10,
		};
		
		class MapSurface : public TGen::Geometry {
		public:	
			MapSurface(const std::string & material);
			~MapSurface();
			
			typedef TGen::Vertex3<float> Vertex;
			typedef TGen::TexCoord2<float, 0> TexCoord;
			typedef TGen::Normal3<float> Normal;
			typedef TGen::VertexAttribute4<TGen::Engine::MapSurfaceTangentAttribute, float> Tangent;
			
			typedef TGen::JoinVertexElements4<Vertex, TexCoord, Normal, Tangent> VertexDecl;
			typedef TGen::Index<unsigned int> IndexDecl;

			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			void createVertexData(TGen::VertexDataSource & source);
			void updateShaderVariable(TGen::ShaderVariable & var, const std::string & name);
			
			TGen::Vector3 getMax() const;
			TGen::Vector3 getMin() const;
			TGen::Vector3 getOrigin() const;
			
			void addVertex(const VertexDecl::Type & vertex);
			void addIndex(const IndexDecl::Type & index);
			void calculateTangents();
			
			std::string getMaterialName() const;
			
		private:
			typedef std::vector<VertexDecl::Type> VertexList;
			typedef std::vector<IndexDecl::Type> IndexList;

			VertexList vertices;
			IndexList indices;
			
			TGen::VertexData * vb, * ib;
			
			std::string material;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MAPSURFACE_H
