/*
 *  mapmodel.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/30/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MAPMODEL_H
#define _TGEN_ENGINE_MAPMODEL_H

#include <string>
#include <vector>
#include <tgen_renderer.h>

namespace TGen {
	class NewFace;
	class RenderList;
	class Camera;
	class MaterialSource;
	class VertexDataSource;
	
	namespace Engine {
		class MapSurface;
		class Map;
		
		class MapModel : public TGen::NewModel {
		public:	
			MapModel(const std::string & name, TGen::Engine::Map * map);
			
			bool isPureInstance() const;
			TGen::NewModelInstance * instantiate(TGen::VertexDataSource & source);
			
			void linkMaterial(TGen::MaterialSource & source);
			void unlinkMaterial();
			void fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, const TGen::SceneNode * node);

			void addSurface(TGen::Engine::MapSurface * surface);
						
			std::string getName() const;
			
		private:
			typedef std::vector<TGen::Engine::MapSurface *> SurfaceList;
			typedef std::vector<TGen::NewFace *> FaceList;
			
			SurfaceList surfaces;
			FaceList faces;
			
			std::string name;
			TGen::Engine::Map * map;
			bool instantiated;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MAPMODEL_H
