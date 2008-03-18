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
	namespace Engine {
		class GameView;
		class World;
		
		class GameState : public TGen::Engine::State {
			bool constructed;
			std::string throttledNewMap;

		public:	
			GameState(TGen::Engine::App & app);
			~GameState();
			
			void tick();
			void render(scalar dt);
			void changeMap(const std::string & mapName);
			
		private:
			void checkErrors();
			
			TGen::Engine::GameStateVars vars;
			TGen::Engine::GameInputMapper inputMapper;
			TGen::Engine::DeferredRenderer * sceneRenderer;
			TGen::Engine::World * currentWorld;
			
			TGen::Time lastRender;
			scalar sinceErrorCheck;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_GAME_H
