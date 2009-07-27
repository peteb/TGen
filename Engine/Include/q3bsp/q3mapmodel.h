/*
 *  q3mapmodel.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/28/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_Q3BSP_Q3MAPMODEL_H
#define _TGEN_ENGINE_Q3BSP_Q3MAPMODEL_H

#include <tgen_renderer.h>

namespace TGen {
	namespace Engine {
		class Q3MapMesh;
		
		class Q3MapModel : public TGen::NewModel {
		public:
			Q3MapModel(const std::string & name);
			
			void addMesh(TGen::Engine::Q3MapMesh * mesh);
			
			bool isPureInstance() const;
			TGen::NewModelInstance * instantiate(TGen::VertexDataSource & source) const;
			void fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, const TGen::SceneNode * node);
			void linkMaterial(TGen::MaterialSource & source);
			
		private:
			std::vector<TGen::Engine::Q3MapMesh *> meshes;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_Q3BSP_Q3MAPMODEL_H

