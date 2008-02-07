/*
 *  md3animmeshinst.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/8/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3ANIMMESHINST_H
#define _TGEN_RENDERER_MD3ANIMMESHINST_H

#include "meshinstance_new.h"

namespace TGen {
	namespace MD3 {
		class AnimatingMesh;
		
		class AnimatingMeshInstance : public TGen::NewMeshInstance {
		public:
			AnimatingMeshInstance(const std::string & materialName, TGen::MD3::AnimatingMesh const & base);
			virtual ~AnimatingMeshInstance();
			
			virtual void updateVertices(int frame, scalar t) abstract;
			virtual void createVertexData(TGen::VertexDataSource & dataSource) abstract;
			
		protected:
			TGen::MD3::AnimatingMesh const & base;
		};
		
		
		
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3ANIMMESHINST_H
