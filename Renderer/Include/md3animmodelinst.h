/*
 *  md3animmodelinst.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3ANIMMODELINST_H
#define _TGEN_RENDERER_MD3ANIMMODELINST_H

#include "modelinstance_new.h"

namespace TGen {
	namespace MD3 {
		class AnimatingModel;
		class AnimatingMeshInstance;
		
		class AnimatingModelInstance : public TGen::NewModelInstance {
		public:
			AnimatingModelInstance(const std::string & name, TGen::MD3::AnimatingModel & base);
			~AnimatingModelInstance();
			
			bool isPureInstance() const;
			void update();
			TGen::ModelJoint getJoint(const std::string & name) const;
			
			void linkMaterial(TGen::MaterialSource & source);
			void unlinkMaterial();
			void fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, TGen::SceneNode const * node);
			
			void addMesh(TGen::MD3::AnimatingMeshInstance * mesh);
			
			int getNumMeshes() const;
			TGen::NewMeshInstance * getMesh(int num);
						
		private:
			TGen::MD3::AnimatingModel & base;
			
			std::vector<TGen::MD3::AnimatingMeshInstance *> meshes;
		};
		
	} // !MD3	
} // !TGen

#endif // !_TGEN_RENDERER_MD3ANIMMODELINST_H
