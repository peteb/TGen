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
		class StaticMesh;
		
		class StaticModel : public TGen::NewModel {
		public:	
			StaticModel(const std::string & name, TGen::VertexDataSource & dataSource, const std::string & materialName, const std::string & materialNamePostfix);		
			~StaticModel();

			TGen::Vector3 getMax() const;
			TGen::Vector3 getMin() const;
			TGen::Vector3 getOrigin() const;
			
			void addJoint(const std::string & name, const TGen::ModelJoint & joint);
			void addMesh(TGen::MD3::StaticMesh * instance);
			
			
			TGen::ModelJoint getJoint(const std::string & name) const;
			TGen::NewMeshInstance * getMesh(int num);
			TGen::NewModelInstance * clone() const;
			
			void writeMeta(uint metaType, const TGen::Matrix4x4 & transform, TGen::VertexStream & stream);
			
			TGen::MD3::StaticModel * instantiate(TGen::VertexDataSource & source) const;

			int getNumMeshes() const;
			bool isPureInstance() const;
			
			void fillFaces(TGen::RenderList & list,  TGen::Material * overridingMaterial, const TGen::SceneNode * node, const TGen::ModelRenderProperties * renderProperties);
			void linkMaterial(TGen::MaterialSource & source);
			void unlinkMaterial();
			void update();
			
		private:
			typedef std::map<std::string, TGen::ModelJoint> JointMap;
			typedef std::vector<TGen::MD3::StaticMesh *> MeshList;
			
			JointMap joints;
			MeshList meshes;
			
			TGen::VertexDataSource & dataSource;
		};
		
	} // !MD3	
} // !TGen

#endif // !_TGEN_RENDERER_MD3STATICMODEL_H

