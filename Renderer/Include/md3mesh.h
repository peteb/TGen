/*
 *  md3mesh.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3MESH_H
#define _TGEN_RENDERER_MD3MESH_H

#include "mesh.h"
#include <tgen_graphics.h>

namespace TGen {
	namespace MD3 {
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
		
		
		
		typedef TGen::JoinVertex3<TGen::Vertex3<float>, TGen::Normal3<float>, TGen::TexCoord2<float, 0> > VertexDecl;
		typedef TGen::Index<unsigned int> IndexDecl;
		
	} // !MD3	
} // !TGen

#endif // !_TGEN_RENDERER_MD3MESH_H

