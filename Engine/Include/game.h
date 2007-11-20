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
#include "deferredscenerenderer.h"
#include "world.h"

namespace TGen {
	namespace Engine {
		class GameView;
		
		class GameState : public TGen::Engine::State {
		public:	
			GameState(TGen::Engine::App & app);
			~GameState();
			
			void tick();
			void render(scalar dt);
			
		private:
			TGen::Engine::GameStateVars vars;
			TGen::Engine::DeferredRenderer sceneRenderer;
			TGen::Engine::World world;
			
			TGen::Time lastRender;
			
			TGen::SceneNode sceneRoot;
			TGen::Camera * camera;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_GAME_H
