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

namespace TGen {
	namespace Engine {
		class GameView;
		
		class GameState : public TGen::Engine::State, public TGen::Engine::VariableObserver {
		public:	
			GameState(TGen::Engine::App & app);
			~GameState();
			
			// State:
			void tick();
			
			// VariableObserver:
			void preVariableChange(const TGen::Engine::Variable & variable, const std::string & newValue);
			void onVariableRemoved(const TGen::Engine::Variable & variable);
			
			
			
			void render(scalar dt);
			
			
		private:
			// Variables:
			void loadVariables();
			
			scalar maxRefreshInterval;
			bool syncVtrace;
			
			
			// Timing members:
			TGen::Time lastRender;
			
			
			
			
			TGen::SceneNode sceneRoot;
			TGen::Camera * camera;
			
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_GAME_H
