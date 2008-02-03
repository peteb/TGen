/*
 *  md5mesh.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/19/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD5MESH_H
#define _TGEN_RENDERER_MD5MESH_H

#include <tgen_graphics.h>
#include "mesh.h"

namespace TGen {
	class Renderer;
	class Camera;
	
	namespace MD5 {
		enum {
			SurfaceTangentAttribute = 10,
			
		};
		
		class Mesh : public TGen::Mesh {
		public:
			Mesh(const std::string & name);
			~Mesh();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			void update(const TGen::Camera & camera, scalar distance, scalar time);
			
			TGen::Vector3 getMax() const;
			TGen::Vector3 getMin() const;
			TGen::Vector3 getOrigin() const;
			
			std::string getDefaultMaterial() const;
		};
		
		typedef TGen::Vertex3<float> Vertex;
		typedef TGen::TexCoord2<float, 0> TexCoord;
		typedef TGen::Normal3<float> Normal;
		typedef TGen::VertexAttribute4<TGen::MD5::SurfaceTangentAttribute, float> Tangent;				// TODO: SurfaceTangentAttribute might be unsycned with engine
		
		typedef TGen::JoinVertexElements4<Vertex, TexCoord, Normal, Tangent> VertexDecl;
		typedef TGen::Index<unsigned int> IndexDecl;
	} // !MD5	
} // !TGen

#endif // !_TGEN_RENDERER_MD5MESH_H