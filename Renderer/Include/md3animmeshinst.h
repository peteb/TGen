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

namespace TGen {
	namespace MD3 {
		class AnimatingMesh;
		
		class AnimatingMeshInstance : public TGen::NewMeshInstance {
		public:	
			AnimatingMeshInstance(const std::string & materialName, TGen::MD3::AnimatingMesh & base);
			~AnimatingMeshInstance();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;
		
		private:
			TGen::MD3::AnimatingMesh & base;
		};
		
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3ANIMMESHINST_H
