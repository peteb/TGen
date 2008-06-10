/*
 *  world.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/16/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_WORLD_H
#define _TGEN_ENGINE_WORLD_H

#include <string>
#include <tgen_renderer.h>
#include "entitylist.h"
#include "renderer/lightlist.h"
#include "entityfactory.h"

#include "scene/subsystem.h"
#include "physics/subsystem.h"
#include "controller/subsystem.h"
#include "sound/subsystem.h"
#include "inventory/subsystem.h"

namespace TGen {
	class Camera;
	class VertexDataSource;
	
	namespace Engine {
		class Filesystem;
		class StandardLogs;
		class ResourceManager;
		class Player;
		class PlayerController;
		
		class World {
		public:
			World(TGen::Engine::Filesystem & filesystem, TGen::Engine::ResourceManager & resources, TGen::Engine::StandardLogs & logs, TGen::VertexDataSource & dataSource, const std::string & mapname);
			~World();
			
			TGen::Camera * getCamera(const std::string & name);
			TGen::RenderList & getRenderList();
			TGen::Engine::LightList & getLightList();
			TGen::Color getAmbientLight();
			TGen::Engine::PlayerController * getPlayerController(const std::string & name);
			
			void prepareLists(TGen::Camera * camera);
			
			void updateListener(const TGen::Vector3 & position, const TGen::Vector3 & velocity, const TGen::Vector3 & forward, const TGen::Vector3 & up);
			void update(scalar dt);
			
		private:
			void loadEntities(const std::string & filename);
			
			TGen::Engine::Filesystem & filesystem;
			TGen::Engine::StandardLogs & logs;
			
			TGen::Engine::Scene::Subsystem sceneSubsystem;
			TGen::Engine::Physics::Subsystem physicsSubsystem;
			TGen::Engine::Controller::Subsystem controllerSubsystem;
			TGen::Engine::Sound::Subsystem soundSubsystem;
			TGen::Engine::Inventory::Subsystem inventorySubsystem;
			
			TGen::Engine::EntityFactory entityFactory;
			
			TGen::SceneNode sceneRoot;
			TGen::BasicRenderList renderList;
			TGen::Engine::LightList lightList;
			TGen::Engine::EntityList entities;
			
			
			
			TGen::Camera * mainCam;
		};
		
	} // Engine
} // !TGen

#endif // !_TGEN_ENGINE_WORLD_H
