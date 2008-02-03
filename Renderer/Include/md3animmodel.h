/*
 *  md3animmodel.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3ANIMMODEL_H
#define _TGEN_RENDERER_MD3ANIMMODEL_H

#include "model_new.h"
#include <tgen_graphics.h>

namespace TGen {
	namespace MD3 {
		class AnimatingMesh;
		
		class AnimatingModel : public TGen::NewModel {
		public:
			AnimatingModel(const std::string & name);
			~AnimatingModel();
			
			void addJoint(const std::string & name, const TGen::ModelJoint & joint);
			void addMeshData(TGen::MD3::AnimatingMesh * mesh);
			
			TGen::ModelJoint getJoint(const std::string & name) const;

			TGen::NewModelInstance * instantiate();
			bool isPureInstance() const;
			
		private:
			typedef std::map<std::string, TGen::ModelJoint> JointMap;
			typedef std::vector<TGen::MD3::AnimatingMesh *> MeshList;
			
			JointMap joints;
			MeshList meshes;			
		};
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3ANIMMODEL_H
