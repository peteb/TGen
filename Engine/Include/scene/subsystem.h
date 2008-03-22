/*
 *  scenesubsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCENE_SUBSYSTEM_H
#define _TGEN_ENGINE_SCENE_SUBSYSTEM_H

#include "../subsystem.h"
#include <tgen_renderer.h>

namespace TGen {
	class VertexDataSource;
	
	namespace Engine {
		class Entity;
		class ResourceManager;
		class Filesystem;
		class StandardLogs;
		
		namespace Scene {
			class Node;
			
			class Subsystem : public TGen::Engine::Subsystem {
			public:
				Subsystem(TGen::Engine::ResourceManager & resources, TGen::Engine::Filesystem & filesystem, TGen::Engine::StandardLogs & logs, TGen::VertexDataSource & dataSource);
				~Subsystem();
				
				TGen::Engine::Component * createComponent(TGen::Engine::Entity * entity, const TGen::PropertyTree & properties);
				void link();
				
				TGen::Engine::Scene::Node * getComponent(const std::string & name);
				TGen::SceneNode & getSceneRoot();
				
			private:
				TGen::SceneNode * createCameraNode(const std::string & name, const TGen::PropertyTree & properties);
				TGen::SceneNode * createLightNode(const std::string & name, const TGen::PropertyTree & properties);
				TGen::SceneNode * createNode(const std::string & name, const TGen::PropertyTree & properties);
				TGen::SceneNode * createMapNode(const std::string & name, const TGen::PropertyTree & properties);
				
				TGen::Engine::ResourceManager & resources;
				TGen::Engine::Filesystem & filesystem;
				TGen::Engine::StandardLogs & logs;
				TGen::VertexDataSource & dataSource;
				
				TGen::SceneNode sceneRoot;
				TGen::MeshGeometryLinkList meshList;
				TGen::ModelInstantiatePool modelPool;
				
				typedef std::map<std::string, TGen::Engine::Scene::Node *> ComponentMap;
				ComponentMap components;
			};
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENE_SUBSYSTEM_H
