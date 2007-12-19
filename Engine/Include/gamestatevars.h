/*
 *  gamestatevars.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_GAMESTATEVARS_H
#define _TGEN_ENGINE_GAMESTATEVARS_H

#include "variable.h"

namespace TGen {
	namespace Engine {
		class App;
		class GameState;
		
		class GameStateVars : public TGen::Engine::VariableObserver {
		public:	
			GameStateVars(TGen::Engine::App & app, TGen::Engine::GameState * state);
			
			void postVariableChange(const TGen::Engine::Variable & variable);
			void onVariableRemoved(const TGen::Engine::Variable & variable);

			void loadVariables();
			
			
			scalar maxRefreshInterval;
			bool syncVtrace, conserveCPU, multithread, checkErrors;
			std::string mapName;
			
		private:
			TGen::Engine::GameState * state;
			TGen::Engine::App & app;
		};
		
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_GAMESTATEVARS_H
