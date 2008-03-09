/*
 *  md5meshinst.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD5MESHINST_H
#define _TGEN_RENDERER_MD5MESHINST_H

#include "meshinstance_new.h"

namespace TGen {	
	namespace MD5 {
		class Mesh;
		
		class MeshInstance : public TGen::NewMeshInstance {
		public:
			MeshInstance(const std::string & materialName, 
							 const std::string & materialNamePostfix, 
							 TGen::MD5::Mesh const & base);
			~MeshInstance();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			
			void updateVertices(int frame, scalar t);
			void updateInterpolatedVertices(int frame1, int frame2, scalar t);
			void createVertexData(TGen::VertexDataSource & dataSource);						
		private:
			TGen::VertexData * vb, * ib;
			TGen::MD5::Mesh const & base;
			
			uint startIndex, numIndices, numVertices;
		};
	} // !MD5
} // !TGen

#endif // !_TGEN_RENDERER_MD5MESHINST_H
