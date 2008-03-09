/*
 *  md5modelinst.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD5MODELINST_H
#define _TGEN_RENDERER_MD5MODELINST_H

#include "modelinstance_new.h"

namespace TGen {
	namespace MD5 {
		class Model;
		class MeshInstance;
		
		class ModelJoint {
		public:	
			ModelJoint(const std::string & name, const TGen::Vector3 & position, const TGen::Quaternion4 & orientation);
			
			void addChild(ModelJoint * child);
			
		private:
			std::vector<TGen::MD5::ModelJoint *> children;
			
			std::string name;
			TGen::Vector3 position;
			TGen::Quaternion4 orientation;
		};
		
		class ModelInstance : public TGen::NewModelInstance {
		public:
			ModelInstance(const std::string & name, const std::string & materialNamePostfix, TGen::MD5::Model & base);
			~ModelInstance();
			
			bool isPureInstance() const;
			/*void update();
			
			void linkMaterial(TGen::MaterialSource & source);
			void unlinkMaterial();
			void fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, TGen::SceneNode const * node);
			*/
			
			void addMesh(TGen::MD5::MeshInstance * mesh);

			int getNumMeshes() const;
			TGen::NewMeshInstance * getMesh(int num);
			void fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, TGen::SceneNode const * node);
			
			// renderlistunit.getGeometry().writeVertices(0 (what to draw), mybatch);
		private:
			TGen::MD5::Model & base;
			
			std::vector<TGen::MD5::MeshInstance *> meshes;
		};
	} // !MD5
} // !TGen

#endif // !_TGEN_RENDERER_MD5MODELINST_H
