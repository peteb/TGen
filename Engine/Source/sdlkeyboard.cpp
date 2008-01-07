/*
 *  sdlkeyboard.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sdlkeyboard.h"
#include <string>

TGen::Engine::SDLKeyboard::SDLKeyboard(int id)
	: TGen::Engine::InputDevice("kbd", id)
{
	controls.numButtons = 255;
}

void TGen::Engine::SDLKeyboard::dispatchEvents(TGen::Engine::InputEventResponder & responder) {

}

std::string TGen::Engine::SDLKeyboard::getDeviceName() {
	return "SDL keyboard";
}
