/*
 *  sdl.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SDL_H
#define _TGEN_ENGINE_SDL_H

#include "environment.h"

namespace TGen {
	class PropertyTree;
	class Renderer;

	namespace Engine {
		class VariablesRegistry;
		class App;
		class StandardLogs;
		
		class SDL : public TGen::Engine::Environment {
		public:
			SDL(TGen::Engine::VariablesRegistry & variables, const TGen::PropertyTree & props, TGen::Engine::StandardLogs & logs);
			~SDL();
	
			int run(TGen::Engine::App * app);
			TGen::Renderer & getRenderer();
			void swapBuffers();
			
		private:
			TGen::Engine::StandardLogs & logs;
			TGen::Engine::VariablesRegistry & variables;			
			TGen::Engine::App * app;
			TGen::Renderer * renderer;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SDL_H

