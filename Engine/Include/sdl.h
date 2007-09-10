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

namespace TGen {
	class PropertyTree;
	
	namespace Engine {
		class VariablesRegistry;
		class App;
		
		class SDL {
		public:
			SDL(TGen::Engine::VariablesRegistry & variables, const TGen::PropertyTree & props);
			~SDL();
	
			int run(TGen::Engine::App * app);
			
		private:
			TGen::Engine::VariablesRegistry & variables;			
			TGen::Engine::App * app;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SDL_H

