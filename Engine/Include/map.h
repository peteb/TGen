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
#include <string>

namespace TGen {
	class VertexDataSource;
	
	namespace Engine {
		class MapModel;
		class MapNode;
		class MapLeafNode;
		
		class Map : public TGen::SceneNode {
		public:
			Map(const std::string & name, const TGen::Vector3 & origin);
			~Map();
			
			
			void addNode(TGen::Engine::MapNode * node);
			void addModel(TGen::Engine::MapModel * model);
			bool fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const;
			bool fillUser(TGen::RenderList & list, const TGen::Camera & camera) const;
			void linkMaterial(TGen::MaterialSource & source);
			void unlinkMaterial();
			void instantiate(TGen::VertexDataSource & source);
			void setNodeRoot(TGen::Engine::MapNode * root);
			
			TGen::Engine::MapNode * getNode(int num);
			TGen::Engine::MapModel * getModel(const std::string & name);
			
		private:
			TGen::Engine::MapLeafNode * getLeafNode(const TGen::Vector3 & position) const;
			
			typedef std::map<std::string, TGen::Engine::MapModel *> ModelMap;
			typedef std::vector<TGen::Engine::MapNode *> NodeMap;
			
			NodeMap nodes;
			ModelMap models;
			TGen::Engine::MapNode * nodeRoot;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MAP_H
