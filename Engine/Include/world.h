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
#include "script/subsystem.h"
#include "info/subsystem.h"
#include "info/worldinfo.h"
#include "utilities/subsystem.h"
#include "resourcemanagerscript.h"
#include "filesystemscript.h"

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
			
			TGen::Engine::PlayerController * getPlayerController();
			TGen::Engine::Scene::Node * getPlayerCamera();
			
			
			void setWorldInfo(TGen::Engine::Info::WorldInfo * worldInfo);
			void prepareLists(TGen::Camera * camera);
			
			void updateListener(const TGen::Vector3 & position, const TGen::Vector3 & velocity, const TGen::Vector3 & forward, const TGen::Vector3 & up);
			void update(scalar dt);
			
			friend class TGen::Engine::Info::WorldInfo;
			
		private:
			void loadEntities(const std::string & filename);
			void loadDefinitions(const std::string & path);
			
			
			TGen::Engine::ResourceManager & resources;
			TGen::Engine::Filesystem & filesystem;
			TGen::Engine::StandardLogs & logs;
			
			
			TGen::Engine::EntityFactory entityFactory;
			
			//TGen::SceneNode sceneRoot;
			TGen::BasicRenderList renderList;
			TGen::Engine::LightList lightList;
			
			TGen::Engine::Script::Subsystem scriptSubsystem;
			TGen::Engine::Scene::Subsystem sceneSubsystem;
			TGen::Engine::Physics::Subsystem physicsSubsystem;
			TGen::Engine::Controller::Subsystem controllerSubsystem;
			TGen::Engine::Sound::Subsystem soundSubsystem;
			TGen::Engine::Inventory::Subsystem inventorySubsystem;
			TGen::Engine::Info::Subsystem infoSubsystem;
			TGen::Engine::Utilities::Subsystem utilsSubsystem;

			TGen::Engine::EntityList entities;

			TGen::Camera * mainCam;
			TGen::Engine::Info::WorldInfo * worldInfo;

			
			TGen::Engine::ResourceManagerScript resourceScript;
			TGen::Engine::FilesystemScript filesystemScript;
		};
		
	} // Engine
} // !TGen

#endif // !_TGEN_ENGINE_WORLD_H
