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

namespace TGen {
	namespace Engine {
		class SDLKeyboard : public TGen::Engine::InputDevice {
		public:
			SDLKeyboard(int id);
			
			void dispatchEvents(TGen::Engine::InputEventResponder & responder);
			std::string getDeviceName();
		};
		
	} // !Engine	
} // !TGen

#endif // !_TGEN_ENGINE_SDLKEYBOARD_H
