/*
 *  md3animmeshdouble.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3ANIMMESHDOUBLE_H
#define _TGEN_RENDERER_MD3ANIMMESHDOUBLE_H

#include "md3struct.h"
#include "md3animmeshinst.h"

namespace TGen {
	class VertexData;
	class VertexDataSource;
	
	namespace MD3 {
		class AnimatingMesh;
		
		class AnimatingMeshDouble : public TGen::MD3::AnimatingMeshInstance {
		public:	
			AnimatingMeshDouble(const std::string & materialName, TGen::MD3::AnimatingMesh const & base);
			~AnimatingMeshDouble();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
			
			void updateVertices(int frame, scalar t);
			void updateDoubleVertices(int frame1, int frame2);
			void createVertexData(TGen::VertexDataSource & dataSource);
			void updateShaderVariable(TGen::ShaderVariable & var, const std::string & name);
			
		private:
			TGen::VertexData * vb, * ib;
			uint startIndex, numIndices, numVertices;
			
			int doubleLastStart, doubleLastEnd;
			scalar dt;
			
			TGen::ShaderVariable * dtVar;
		};
		
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3ANIMMESHINST_H