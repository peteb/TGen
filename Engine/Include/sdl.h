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
		class SDLKeyboard;
		class SDLMouse;
		class SDLGamepad;
		
		class SDL : public TGen::Engine::Environment {
		public:
			SDL(TGen::Engine::VariablesRegistry & variables, const TGen::PropertyTree & props, TGen::Engine::StandardLogs & logs);
			~SDL();
	
			int run(TGen::Engine::App * app);
			TGen::Renderer & getRenderer();
			void swapBuffers();
			void registerInputDevices(TGen::Engine::DeviceCollection & inputDevices);
			
		private:
			TGen::Engine::StandardLogs & logs;
			TGen::Engine::VariablesRegistry & variables;			
			TGen::Engine::App * app;
			TGen::Renderer * renderer;
			
			TGen::Engine::SDLKeyboard * keyboard;
			TGen::Engine::SDLMouse * mouse;
			
			std::vector<TGen::Engine::SDLGamepad *> gamepads;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SDL_H

