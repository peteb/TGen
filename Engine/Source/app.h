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

namespace TGen {
	namespace Engine {
		class SDL;
		class Filesystem;
		class VariablesRegistry;
		
		class App {
		public:
			App(TGen::Engine::VariablesRegistry & variables, TGen::Engine::SDL * sdl, TGen::Engine::Filesystem * fs);
			~App();
			
			bool isRunning() const;
			void quit();
			void tick();
			
		private:
			bool running;
		};
		
		
	} // !Engine	
} // !TGen

#endif // !_TGEN_ENGINE_APP_H

