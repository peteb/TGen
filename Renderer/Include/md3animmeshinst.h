/*
 *  md3animmeshinst.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3ANIMMESHINST_H
#define _TGEN_RENDERER_MD3ANIMMESHINST_H

#include "meshinstance_new.h"
#include "md3struct.h"

namespace TGen {
	class VertexData;
	class VertexDataSource;
	
	namespace MD3 {
		class AnimatingMesh;
		
		// TODO: egen klass för double vertices
		
		class AnimatingMeshInstance : public TGen::NewMeshInstance {
		public:	
			AnimatingMeshInstance(const std::string & materialName, bool doubleVertices, TGen::MD3::AnimatingMesh & base);
			~AnimatingMeshInstance();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			
			void updateVertices(int frame, scalar t);
			void updateInterpolatedVertices(int frame1, int frame2, scalar t);
			void updateDoubleVertices(int frame1, int frame2);
			void createVertexData(TGen::VertexDataSource & dataSource);
			
			TGen::VertexData * vb, * ib;
			TGen::PrimitiveType primitive;
			uint startIndex, indexCount;
			
		private:
			bool doubleVertices;
			int doubleLastStart, doubleLastEnd;
			TGen::MD3::AnimatingMesh & base;
		};
		
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3ANIMMESHINST_H
