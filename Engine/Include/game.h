/*
 *  game.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_GAME_H
#define _TGEN_ENGINE_GAME_H

#include <tgen_renderer.h>
#include "state.h"
#include "variable.h"
#include "gamestatevars.h"
#include "renderer/deferred/renderer.h"
#include "gameinputmapper.h"

namespace TGen {
	class VertexDataSoure;
	
	namespace Engine {
		class GameView;
		class World;
		class WorldRenderer;
		class StandardLogs;
		class VariableRegister;
		class Filesystem;
		class DeviceCollection;
		class Environment;
		class ResourceManager;
		
		class GameState : public TGen::Engine::State {
			bool constructed;
			std::string throttledNewMap;

		public:	
			GameState(TGen::Engine::DeviceCollection & inputDevices, TGen::Engine::Environment & env, TGen::Engine::Filesystem & filesystem, TGen::Engine::VariableRegister & variables, TGen::Engine::StandardLogs & logs, TGen::Engine::WorldRenderer & worldRenderer, TGen::Engine::ResourceManager & resources, TGen::VertexDataSource & dataSource);
			~GameState();
			
			void tick();
			void render(scalar dt);
			void changeMap(const std::string & mapName);
			
		private:
			void checkErrors();
			
			TGen::Engine::DeviceCollection & inputDevices;
			TGen::Engine::Filesystem & filesystem;
			TGen::Engine::Environment & env;
			TGen::Engine::GameStateVars vars;
			TGen::Engine::GameInputMapper inputMapper;
			TGen::Engine::WorldRenderer & worldRenderer;
			TGen::Engine::World * currentWorld;
			TGen::Engine::StandardLogs & logs;
			TGen::Engine::ResourceManager & resources;
			TGen::VertexDataSource & dataSource;
			
			TGen::Time lastRender;
			scalar sinceErrorCheck;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_GAME_H
