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
	, mode(TGen::Engine::DefaultMode)
{
	controls.numButtons = 255;
}

void TGen::Engine::SDLKeyboard::dispatchEvents(TGen::Engine::InputEventResponder & responder) {
	if (mode == TGen::Engine::TextMode) {
		if (!textBuffer.empty()) {
			responder.onTextEvent(*this, textBuffer);
			textBuffer = "";
		}
	}
	else {
		
	}
}

std::string TGen::Engine::SDLKeyboard::getDeviceName() {
	return "SDL keyboard";
}

void TGen::Engine::SDLKeyboard::onBinaryEvent(const SDL_keysym & keysym, TGen::Engine::StateEvent state) {
	//std::cout << "got event " << char(keysym.unicode) << std::endl;
	
	if (mode == TGen::Engine::TextMode) {
		if (state == TGen::Engine::StateDown) {
			char asciiCode = keysym.unicode & 0x7F;
			
			if (keysym.unicode > 0x7F)
				asciiCode = 0;
			
			if (asciiCode == 0x7F)
				asciiCode = '\r';
			
			switch (keysym.sym) {
				case SDLK_DELETE:
					asciiCode = '\r';
					break;
					
				case SDLK_RETURN:
					asciiCode = '\n';
					break;
				
				case SDLK_LEFT:
					asciiCode = TGen::Engine::SpecialLeft;
					break;
					
				case SDLK_RIGHT:
					asciiCode = TGen::Engine::SpecialRight;
			}
			
			if (asciiCode != 0)
				textBuffer += asciiCode;
		}
	}
	else {
		
	}
	
	// aktivera TextMode för text, annars kommer inga sånna events!!!!!!!!!!!!!!
	// text events buffrar till en sträng, text responder måste alltså ta emot en sträng
	// config kan be varje device att konvertera ett namn, typ "LEFT" till en button
}

void TGen::Engine::SDLKeyboard::enterMode(TGen::Engine::InputDeviceMode mode) {
	if (mode == this->mode)
		return;
	
	if (mode == TGen::Engine::TextMode) {
		SDL_EnableUNICODE(SDL_TRUE);
		SDL_EnableKeyRepeat(155, SDL_DEFAULT_REPEAT_INTERVAL);
		std::cout << "ENTERED TEXT MODE" << std::endl;
	}
	else {
		SDL_EnableUNICODE(SDL_FALSE);
		SDL_EnableKeyRepeat(0, 0);
		textBuffer = "";
		std::cout << "LEFT TEXT MODE" << std::endl;
	}
	
	this->mode = mode;
}
