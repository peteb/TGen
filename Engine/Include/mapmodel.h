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
		class MapPortal;
		
		class MapModel : public TGen::NewModel, public TGen::SceneNode {
		public:	
			MapModel(const std::string & name, TGen::Engine::Map * map);
			
			void addPortal(TGen::Engine::MapPortal * portal);
			int getNumPortals() const;
			TGen::Engine::MapPortal * getPortal(int num);
			
			bool isPureInstance() const;
			TGen::NewModelInstance * instantiate(TGen::VertexDataSource & source);
			
			void linkMaterial(TGen::MaterialSource & source);
			void unlinkMaterial();
			void fillFaces(TGen::RenderList & list, TGen::Material * overridingMaterial, const TGen::SceneNode * node);

			void addSurface(TGen::Engine::MapSurface * surface);
			void writeMeta(uint metaType, const TGen::Matrix4x4 & transform, TGen::VertexStream & stream);
			
			std::string getName() const;
			
			void traverse(const TGen::SceneNode::Walker & walker);
			
		private:
			typedef std::vector<TGen::Engine::MapSurface *> SurfaceList;
			typedef std::vector<TGen::Engine::MapPortal *> PortalList;
			
			PortalList portals;
			SurfaceList surfaces;
			
			std::string name;
			TGen::Engine::Map * map;
			bool instantiated;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MAPMODEL_H
