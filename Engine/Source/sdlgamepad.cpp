/*
 *  sdlgamepad.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sdlgamepad.h"

TGen::Engine::SDLGamepad::SDLGamepad(int id, int internalId)
	: TGen::Engine::InputDevice("gamepad", id)
	, joystick(NULL)
{
	joystick = SDL_JoystickOpen(internalId);
	
	if (!joystick)
		throw TGen::RuntimeException("SDLGamepad::SDLGamepad", "failed to open joystick # ") << internalId << "!";
	
	joyName = SDL_JoystickName(internalId);
	
	controls.numAxes = SDL_JoystickNumAxes(joystick);
	controls.numButtons = SDL_JoystickNumBalls(joystick);
	controls.numBalls = SDL_JoystickNumBalls(joystick);
	controls.numHats = SDL_JoystickNumHats(joystick);
}

TGen::Engine::SDLGamepad::~SDLGamepad() {
	SDL_JoystickClose(joystick);
}

void TGen::Engine::SDLGamepad::dispatchEvents(TGen::Engine::InputEventResponder & responder) {

}

std::string TGen::Engine::SDLGamepad::getDeviceName() {
	return joyName;
}
