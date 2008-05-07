/*
 *  map.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/29/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MAP_H
#define _TGEN_ENGINE_MAP_H

#include <tgen_renderer.h>

namespace TGen {
	class VertexDataSource;
	
	namespace Engine {
		class MapModel;
		
		class Map : public TGen::SceneNode {
		public:
			Map(const std::string & name, const TGen::Vector3 & origin);
			
			void addModel(TGen::Engine::MapModel * model);
			bool fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const;
			bool fillUser(TGen::RenderList & list, const TGen::Camera & camera) const;
			void linkMaterial(TGen::MaterialSource & source);
			void unlinkMaterial();
			void instantiate(TGen::VertexDataSource & source);
			
		private:
			typedef std::map<std::string, TGen::Engine::MapModel *> ModelMap;
			
			ModelMap models;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MAP_H
