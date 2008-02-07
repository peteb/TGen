/*
 *  md3animmeshsingle.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3ANIMMESHSINGLE_H
#define _TGEN_RENDERER_MD3ANIMMESHSINGLE_H

#include "md3struct.h"
#include "md3animmeshinst.h"

namespace TGen {
	class VertexData;
	class VertexDataSource;
	
	namespace MD3 {
		class AnimatingMesh;
		
		class AnimatingMeshSingle : public TGen::MD3::AnimatingMeshInstance {
		public:	
			AnimatingMeshSingle(const std::string & materialName, TGen::MD3::AnimatingMesh const & base);
			~AnimatingMeshSingle();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			
			void updateVertices(int frame, scalar t);
			void updateInterpolatedVertices(int frame1, int frame2, scalar t);
			void createVertexData(TGen::VertexDataSource & dataSource);

		private:
			TGen::VertexData * vb, * ib;
			uint startIndex, numIndices, numVertices;
		};
		
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3ANIMMESHSINGLE_H
