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
			AnimatingModelInstance(const std::string & name, const std::string & materialNamePostfix, TGen::MD3::AnimatingModel & base);
			~AnimatingModelInstance();
			
			bool isPureInstance() const;
			void update();
			TGen::ModelJoint getJoint(const std::string & name) const;
			
			void linkMaterial(TGen::MaterialSource & source);
			void unlinkMaterial();
			void fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, TGen::SceneNode const * node);

			TGen::NewModelInstance * clone() const;
			
			void addMesh(TGen::MD3::AnimatingMeshInstance * mesh);
			void writeMeta(uint metaType, const TGen::Matrix4x4 & transform, TGen::VertexStream & stream);
			
			int getNumMeshes() const;
			TGen::NewMeshInstance * getMesh(int num);
						
		private:
			TGen::MD3::AnimatingModel & base;
			TGen::MaterialSource * materialSource;
			
			std::vector<TGen::MD3::AnimatingMeshInstance *> meshes;
			
			
			float num;		// TODO: animation time. should be fixed!
		};
		
	} // !MD3	
} // !TGen

#endif // !_TGEN_RENDERER_MD3ANIMMODELINST_H
