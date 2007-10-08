/*
 *  app.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_APP_H
#define _TGEN_ENGINE_APP_H

#include "log.h"
#include "logtarget.h"

namespace TGen {
	class PropertyTree;
	class Renderer;
	
	namespace Engine {
		class SDL;
		class Filesystem;
		class VariablesRegistry;
		class State;
		class Environment;
		
		class App {
		public:
			App(TGen::Engine::VariablesRegistry & variables, TGen::Engine::Environment & env, TGen::Engine::Filesystem * fs, 
				const TGen::PropertyTree & props, TGen::Renderer & renderer, TGen::Engine::Log & info, TGen::Engine::Log & warning, TGen::Engine::Log & error);
			~App();
			
			bool isRunning() const;
			void quit();
			void tick();
			
			TGen::Engine::Log & info, & warning, & error;
			TGen::Engine::Environment & env;
			TGen::Engine::VariablesRegistry & variables;
			TGen::Engine::Filesystem & filesystem;
			const TGen::PropertyTree & loadProps;
			TGen::Renderer & renderer;
			
		private:
			TGen::Engine::State * currentState;
			bool running;
		};
		
		
	} // !Engine	
} // !TGen

#endif // !_TGEN_ENGINE_APP_H

