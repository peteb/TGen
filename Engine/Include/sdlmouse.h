/*
 *  sdlmouse.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SDLMOUSE_H
#define _TGEN_ENGINE_SDLMOUSE_H

#include "inputdevice.h"
#include <tgen_math.h>

struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;

namespace TGen {
	namespace Engine {
		class SDLMouse : public TGen::Engine::InputDevice {
		public:
			SDLMouse(int id);
			
			void dispatchEvents(TGen::Engine::InputEventResponder & responder);
			void enterMode(TGen::Engine::InputDeviceMode mode);
			std::string getDeviceName();
			
			void onMotionEvent(const SDL_MouseMotionEvent & motion);
			void onButtonEvent(const SDL_MouseButtonEvent & button);
			
		private:
			std::vector<uint8> buttons;
			
			TGen::Engine::InputDeviceMode mode;
			TGen::Vector3 position, scroll;
			bool positionChanged, scrollChanged;
		};
		
	} // !Engine	
} // !TGen

#endif // !_TGEN_ENGINE_SDLMOUSE_H
