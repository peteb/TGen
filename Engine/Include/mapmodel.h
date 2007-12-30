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

namespace TGen {
	class Face;
	class RenderList;
	class Camera;
	class MaterialSource;
	class VertexDataSource;
	
	namespace Engine {
		class MapSurface;
		class Map;
		
		class MapModel {
		public:	
			MapModel(const std::string & name, TGen::Engine::Map * map);
			
			void addSurface(TGen::Engine::MapSurface * surface);
			bool fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const;
			void linkMaterials(TGen::MaterialSource & source);
			void createVertexData(TGen::VertexDataSource & source);
			
			std::string getName() const;
			
		private:
			typedef std::vector<TGen::Engine::MapSurface *> SurfaceList;
			typedef std::vector<TGen::Face *> FaceList;
			
			SurfaceList surfaces;
			FaceList faces;
			
			std::string name;
			TGen::Engine::Map * map;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MAPMODEL_H
