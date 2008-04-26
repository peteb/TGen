/*
 *  cubemodel.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_CUBEMODEL_H
#define _TGEN_ENGINE_CUBEMODEL_H

#include <tgen_renderer.h>
#include <tgen_math.h>

namespace TGen {
	namespace Engine {
		class CubeMesh;
		
		class CubeModel : public TGen::NewModel {
		public:
			CubeModel(const std::string & name, const std::string & materialName, const std::string & materialNamePostfix, const TGen::Vector3 & min, const TGen::Vector3 & max);
			~CubeModel();
			
			TGen::Vector3 getMax() const;
			TGen::Vector3 getMin() const;
			
			void addMesh(TGen::Engine::CubeMesh * instance);

			TGen::NewMeshInstance * getMesh(int num);
			int getNumMeshes() const;
			
			TGen::Engine::CubeModel * instantiate();
			
			bool isPureInstance() const;
			
			void fillFaces(TGen::RenderList & list,  TGen::Material * overridingMaterial, const TGen::SceneNode * node);			
			void linkMaterial(TGen::MaterialSource & source);
			void unlinkMaterial();
			void writeMeta(uint metaType, const TGen::Matrix4x4 & transform, TGen::VertexStream & stream);
			
			//void update();
			
		private:
			typedef std::vector<TGen::Engine::CubeMesh *> MeshList;
			
			MeshList meshes;			
			TGen::Vector3 min, max;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CUBEMODEL_H

