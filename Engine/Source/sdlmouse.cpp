/*
 *  sdlmouse.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sdlmouse.h"

TGen::Engine::SDLMouse::SDLMouse(int id)
	: TGen::Engine::InputDevice("mouse", id)
{
	controls.numButtons = 5;
	controls.numAxes = 2;
}

void TGen::Engine::SDLMouse::dispatchEvents(TGen::Engine::InputEventResponder & responder) {

}

std::string TGen::Engine::SDLMouse::getDeviceName() {
	return "SDL mouse";
}
