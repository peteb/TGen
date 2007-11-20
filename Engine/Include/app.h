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
#include "resourcemanager.h"

namespace TGen {
	class PropertyTree;
	class Renderer;
	
	namespace Engine {
		class SDL;
		class Filesystem;
		class VariablesRegistry;
		class CommandRegistry;
		class State;
		class Environment;
	
		class App {
		public:
			App(TGen::Engine::VariablesRegistry & variables, TGen::Engine::CommandRegistry & commands, TGen::Engine::Environment & env, 
				TGen::Engine::Filesystem * fs, const TGen::PropertyTree & props, TGen::Renderer & renderer, TGen::Engine::StandardLogs & logs);
			~App();
			
			bool isRunning() const;
			void quit();
			void tick();
			
			TGen::Time startedAt;
			
			TGen::Engine::StandardLogs & logs;
			TGen::Engine::Environment & env;
			TGen::Engine::VariablesRegistry & variables;
			TGen::Engine::CommandRegistry & commands;
			TGen::Engine::Filesystem & filesystem;
			
			TGen::Engine::ResourceManager globalResources;
			
			const TGen::PropertyTree & loadProps;
			TGen::Renderer & renderer;
			
		private:
			TGen::Engine::State * currentState;
			bool running;
		};
		
		
	} // !Engine	
} // !TGen

#endif // !_TGEN_ENGINE_APP_H

