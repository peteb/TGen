/*
 *  sdlkeyboard.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SDLKEYBOARD_H
#define _TGEN_ENGINE_SDLKEYBOARD_H

#include "inputdevice.h"
#include "inputeventresponder.h"

struct SDL_keysym;

namespace TGen {
	namespace Engine {
		class SDLKeyboard : public TGen::Engine::InputDevice {
		public:
			SDLKeyboard(int id);
			
			void onBinaryEvent(const SDL_keysym & keysym, TGen::Engine::StateEvent state);
			void enterMode(TGen::Engine::InputDeviceMode mode);
			void reset();
			
			void dispatchEvents(TGen::Engine::InputEventResponder & responder);
			std::string getDeviceName();
			
		private:
			TGen::Engine::InputDeviceMode mode;
			std::string textBuffer;
			std::vector<uint16> buttonBuffer;
		};
		
	} // !Engine	
} // !TGen

#endif // !_TGEN_ENGINE_SDLKEYBOARD_H
