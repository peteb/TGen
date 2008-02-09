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
	class VertexDataSource;
	
	namespace MD3 {
		class AnimatingMesh;
		
		enum InterpolatingMode {
			CPU,
			GPU,
		};
		
		class AnimatingModel : public TGen::NewModel {
		public:
			AnimatingModel(const std::string & name, TGen::VertexDataSource & dataSource);
			~AnimatingModel();
			
			void addJoint(const std::string & name, const TGen::ModelJoint & joint);
			void addMeshData(TGen::MD3::AnimatingMesh * mesh);
			void setInterpolatingMode(TGen::MD3::InterpolatingMode mode);
			
			TGen::ModelJoint getJoint(const std::string & name) const;
			
			TGen::NewModelInstance * instantiate();
			bool isPureInstance() const;
			
		private:
			typedef std::map<std::string, TGen::ModelJoint> JointMap;
			typedef std::vector<TGen::MD3::AnimatingMesh *> MeshList;
			
			JointMap joints;
			MeshList meshes;	
			
			TGen::VertexDataSource & dataSource;
			
			TGen::MD3::InterpolatingMode interpolatingMode;
		};
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3ANIMMODEL_H
