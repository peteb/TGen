/*
 *  md3staticmodel.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Renamed by Peter Backman on 2/3/08.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3STATICMODEL_H
#define _TGEN_RENDERER_MD3STATICMODEL_H

#include "model_new.h"
#include <tgen_graphics.h>

namespace TGen {
	namespace MD3 {
		class Mesh;
		
		class StaticModel : public TGen::NewModel {
		public:	
			StaticModel(const std::string & name);		
			~StaticModel();

			//void preRender(TGen::Renderer & renderer) const;
			//void render(TGen::Renderer & renderer) const;
			//void update(const TGen::Camera & camera, scalar distance, scalar time);
			
			TGen::Vector3 getMax() const;
			TGen::Vector3 getMin() const;
			TGen::Vector3 getOrigin() const;
			
			void addJoint(const std::string & name, const TGen::ModelJoint & joint);
			void addMesh(TGen::MD3::Mesh * instance);

			TGen::ModelJoint getJoint(const std::string & name) const;
			TGen::NewMeshInstance * getMesh(int num);
			
			TGen::MD3::StaticModel * instantiate();

			int getNumMeshes() const;
			bool isPureInstance() const;
			
			void fillFaces(TGen::RenderList & list,  TGen::Material * overridingMaterial, TGen::SceneNode const * node);
			void linkMaterial(TGen::MaterialSource & source);
			void unlinkMaterial();
			void update();
			
		private:
			typedef std::map<std::string, TGen::ModelJoint> JointMap;
			typedef std::vector<TGen::MD3::Mesh *> MeshList;
			
			JointMap joints;
			MeshList meshes;
		};
		
	} // !MD3	
} // !TGen

#endif // !_TGEN_RENDERER_MD3STATICMODEL_H

