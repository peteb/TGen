/*
 *  sdlgamepad.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SDLGAMEPAD_H
#define _TGEN_ENGINE_SDLGAMEPAD_H

#include "inputdevice.h"
#include <SDL/SDL.h>

namespace TGen {
	namespace Engine {
		class SDLGamepad : public TGen::Engine::InputDevice {
		public:
			SDLGamepad(int id, int internalId);
			~SDLGamepad();
			
			void dispatchEvents(TGen::Engine::InputEventResponder & responder);
			std::string getDeviceName();
			
		private:
			SDL_Joystick * joystick;
			std::string joyName;
		};
		
	} // !Engine	
} // !TGen

#endif // !_TGEN_ENGINE_SDLGAMEPAD_H
