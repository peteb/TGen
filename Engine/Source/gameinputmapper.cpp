/*
 *  gameinputmapper.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/9/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "gameinputmapper.h"
#include <iostream>
#include <sstream>
#include <tgen_math.h>
#include "inputdevice.h"

TGen::Engine::GameInputMapper::GameInputMapper() {
	
}

TGen::Engine::GameInputMapper::~GameInputMapper() {
	
}

void TGen::Engine::GameInputMapper::onBinaryEvent(TGen::Engine::InputDevice & device, int id, TGen::Engine::StateEvent state) {
	std::stringstream deviceName;
	deviceName << device.getName() << device.getId() << " (" << device.getDeviceName() << ")";
	
	if (state == TGen::Engine::StateDown)
		std::cout << "*** button down on " << deviceName.str() << ": " << id << std::endl;
	else
		std::cout << "*** button up on " << deviceName.str() << ": " << id << std::endl;

}

void TGen::Engine::GameInputMapper::onTextEvent(TGen::Engine::InputDevice & device, const std::string & text) {
	for (int i = 0; i < text.size(); ++i) {
		// det här borde skickas till den textruta som är in-focus
		
		if ((unsigned char)(text[i]) <= 127) {
			std::cout << int(text[i]) << std::endl;
			if (text[i] != '\r')
				this->text += text[i];
			else
				this->text = this->text.substr(0, this->text.size() - 1);
		}
	}
	
	std::cout << "'" << this->text << "'" << std::endl;
}

void TGen::Engine::GameInputMapper::onVectorEvent(TGen::Engine::InputDevice & device, int id, const TGen::Vector3 & vec) {
	std::stringstream deviceName;
	deviceName << device.getName() << device.getId() << " (" << device.getDeviceName() << ")";

	std::cout << "*** vector move on " << deviceName.str() << " id: " << id << ": " << std::string(vec) << std::endl;
}

