/*
 *  sdlmouse.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sdlmouse.h"
#include "inputeventresponder.h"
#include <SDL/SDL.h>

TGen::Engine::SDLMouse::SDLMouse(int id)
	: TGen::Engine::InputDevice("mouse", id)
	, positionChanged(false)
	, scrollChanged(false)
	, mode(TGen::Engine::AbsoluteMode)
	, buttons(10, 0)
{
	controls.numButtons = 10;
	controls.numAxes = 2;
	
	SDL_ShowCursor(SDL_FALSE);
}

void TGen::Engine::SDLMouse::dispatchEvents(TGen::Engine::InputEventResponder & responder) {
	if (positionChanged) {
		responder.onVectorEvent(*this, 0, position);
		positionChanged = false;
		
		position = TGen::Vector3::Zero;
	}
	
	if (scrollChanged) {
		responder.onVectorEvent(*this, 1, scroll);
		scrollChanged = false;
		
		if (mode == TGen::Engine::RelativeMode)
			scroll = TGen::Vector3::Zero;
	}
	
	for (int i = 0; i < buttons.size(); ++i) {
		if (buttons[i] & 0x01)
			responder.onBinaryEvent(*this, i, TGen::Engine::StateDown);
		if (buttons[i] & 0x02)
			responder.onBinaryEvent(*this, i, TGen::Engine::StateUp);
		
		buttons[i] = 0;
	}
}

std::string TGen::Engine::SDLMouse::getDeviceName() {
	return "SDL mouse";
}

void TGen::Engine::SDLMouse::onMotionEvent(const SDL_MouseMotionEvent & motion) {
	if (mode == TGen::Engine::AbsoluteMode) {
		position = TGen::Vector3(motion.x, motion.y, 0.0f);
	}
	else if (mode == TGen::Engine::RelativeMode) {
		position += TGen::Vector3(motion.xrel, motion.yrel, 0.0f);
	}
	
	positionChanged = true;
}

void TGen::Engine::SDLMouse::enterMode(TGen::Engine::InputDeviceMode mode) {
	if (mode == this->mode)
		return;
	
	if (mode == TGen::Engine::RelativeMode) {
		//SDL_WM_GrabInput(SDL_GRAB_ON);

		std::cout << "ENTERED RELATIVE MODE" << std::endl;
	}
	else if (mode == TGen::Engine::AbsoluteMode || mode == TGen::Engine::DefaultMode) {
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		
		std::cout << "ENTERED ABSOLUTE MODE" << std::endl;
	}
	
	this->mode = mode;
}

void TGen::Engine::SDLMouse::onButtonEvent(const SDL_MouseButtonEvent & button) {
	int buttonId = button.button - 1;
	
	switch (button.button) {
		case SDL_BUTTON_LEFT:
			buttonId = 0;
			break;
			
		case SDL_BUTTON_RIGHT:
			buttonId = 1;
			break;
			
		case SDL_BUTTON_MIDDLE:
			buttonId = 2;
			break;
			
		case SDL_BUTTON_WHEELUP:
			buttonId = 3;
			
			if (button.state == SDL_PRESSED) {
				scroll += TGen::Vector3(1.0f, 0.0f, 0.0f);
				scrollChanged = true;
			}
			
			break;
			
		case SDL_BUTTON_WHEELDOWN:
			buttonId = 4;
			
			if (button.state == SDL_PRESSED) {
				scroll -= TGen::Vector3(1.0f, 0.0f, 0.0f);
				scrollChanged = true;
			}

			break;
			
		default:
			std::cout << "weird button: " << int(button.button) << std::endl;
	}
	
	uint8 state = 0;
	
	// TODO: possible bug, what if the player sends MOUSE_UP then MOUSE_DOWN faster than the game, you get MOUSE_DOWN MOUSE_UP
	if (button.state == SDL_PRESSED)
		state = 0x01;
	else if (button.state == SDL_RELEASED)
		state = 0x02;
	
	if (buttons.size() < buttonId) {
		for (int i = 0; i < buttonId - buttons.size(); ++i)
			buttons.push_back(0);
	}
	
	buttons[buttonId] |= state;
}
