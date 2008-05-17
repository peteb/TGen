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
		class MapLinkNode;
		class MapLeafNode;
		class MapPortal;
		
		class Map : public TGen::SceneNode {
		public:
			Map(const std::string & name, const TGen::Vector3 & origin);
			~Map();
			
			
			void addNode(TGen::Engine::MapLinkNode * node);
			void addModel(TGen::Engine::MapModel * model);
			void addPortal(TGen::Engine::MapPortal * portal);
			
			bool fillFaces(TGen::RenderList & list, const TGen::Camera & camera) const;
			bool fillUser(TGen::RenderList & list, const TGen::Camera & camera) const;
			void linkMaterial(TGen::MaterialSource & source);
			void unlinkMaterial();
			void instantiate(TGen::VertexDataSource & source);
			void setRootNode(TGen::Engine::MapLinkNode * root);
			bool fillMeta(TGen::RenderList & list, const TGen::Camera & camera) const;
			void traverse(const TGen::SceneNode::Walker & walker);
			
			
			TGen::Engine::MapLinkNode * getNode(int num);
			TGen::Engine::MapModel * getModel(const std::string & name);
			TGen::SceneNode * getNodeFromPoint(const TGen::Vector3 & point);
			
		private:
			TGen::Engine::MapLeafNode * getLeafNode(const TGen::Vector3 & position) const;
			void fillModels(TGen::Engine::MapModel * leaf, TGen::RenderList & list, const TGen::Camera & camera, const TGen::Rectangle & clipRect) const;
			
			typedef std::map<std::string, TGen::Engine::MapModel *> ModelMap;
			typedef std::vector<TGen::Engine::MapLinkNode *> NodeMap;
			typedef std::vector<TGen::Engine::MapPortal *> PortalList;
			typedef std::map<TGen::Engine::MapModel *, bool> ModelRenderedMap;
			
			mutable ModelRenderedMap modelRendered;
			PortalList portals;
			NodeMap nodes;
			ModelMap models;
			TGen::Engine::MapLinkNode * rootNode;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MAP_H
