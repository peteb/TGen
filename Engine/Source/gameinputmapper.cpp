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
#include <tgen_renderer.h>
#include "inputdevice.h"
#include "playercontroller.h"

TGen::Engine::GameInputMapper::GameInputMapper()
	: playerController(NULL)
	, initialBump(false)
{
	
}

TGen::Engine::GameInputMapper::~GameInputMapper() {
	
}

void TGen::Engine::GameInputMapper::onBinaryEvent(TGen::Engine::InputDevice & device, int id, TGen::Engine::StateEvent state) {
	std::stringstream deviceName;
	deviceName << device.getName() << device.getId() << " (" << device.getDeviceName() << ")";
	
	if (state == TGen::Engine::StateDown)
		std::cout << "*** button down on '" << deviceName.str() << "': " << id << std::endl;
	else
		std::cout << "*** button up on '" << deviceName.str() << "': " << id << std::endl;

	int eventId = -1;

	if (device.getName() == "mouse") {
		switch (id) {
			case 0:
				eventId = TGen::Engine::EventPrimaryFire;
				break;
				
			case 1:
				eventId = TGen::Engine::EventSecondaryFire;
				break;
				
			case 3:
				eventId = TGen::Engine::EventPreviousWeapon;
				break;
				
			case 4:
				eventId = TGen::Engine::EventNextWeapon;
				break;
		}
	}
	else {
		eventId = keyToEventID(id);
	}
	
	if (playerController) {
		if (state == TGen::Engine::StateDown)
			playerController->beginEvent(eventId);
		else if (state == TGen::Engine::StateUp)
			playerController->endEvent(eventId);	
	}
}

int TGen::Engine::GameInputMapper::keyToEventID(int id) const {
	switch (id) {
		case 'w':
			return TGen::Engine::EventForward;
			
		case 's':
			return TGen::Engine::EventBackward;
			
		case 'a':
			return TGen::Engine::EventStrafeLeft;
			
		case 'd':
			return TGen::Engine::EventStrafeRight;
			
		case 306:
			return TGen::Engine::EventCrouch;
			
		case ' ':
			return TGen::Engine::EventJump;
	}
	
	return -1;
}


void TGen::Engine::GameInputMapper::onTextEvent(TGen::Engine::InputDevice & device, const std::string & text) {
	for (int i = 0; i < text.size(); ++i) {
		// det här borde skickas till den textruta som är in-focus, sen buffras det där
		
		if ((unsigned char)(text[i]) <= 127) {
			std::cout << int(text[i]) << std::endl;
			if (text[i] != '\r')
				this->text += text[i];
			else
				this->text = this->text.substr(0, this->text.size() - 1);
		}
	}
	
	// GÖR PLAYER, ska ha en pekare till kameran
	// controller sitter på player direkt typ, Player::getController, om playern är i ghost mode så returneras ghostcontroller
	
	// world.getPlayer().getController().move(blabla);
	
	// GameInputMapper tar referens till controller i constructor, styr på det viset
	//   controllern är en referens, dvs, operator *. TGen::deref. Så kan player ärva controller och på så vis returnera korrekt controller för läget den är i
	//   då slipper man skapa ny inputmapper om man dör eller nått. Men gör inte det direkt, man kanske inte behöver byta controller! testa testa
	
	
	// gamestate -> mapper -> controller -> camera
	
	std::cout << "Text: '" << this->text << "'" << std::endl;
	
	//device.reset();
}

void TGen::Engine::GameInputMapper::onVectorEvent(TGen::Engine::InputDevice & device, int id, const TGen::Vector3 & vec) {
	//std::stringstream deviceName;
	//deviceName << device.getName() << device.getId() << " (" << device.getDeviceName() << ")";

	//std::cout << "*** vector move on " << deviceName.str() << " id: " << id << ": " << std::string(vec) << std::endl;
	

	if (playerController && playerController->useRelativeView())
		device.enterMode(TGen::Engine::RelativeMode);
	else
		device.enterMode(TGen::Engine::AbsoluteMode);
	
	// if (playerController->useAbsoluteView()) device.setAbsoluteView()...... else device.setRelativeView()
	if (vec.getMagnitude() > 100.0f && !initialBump) {
		initialBump = true;
		return;
	}
	
	if (playerController && id == 0)
		playerController->addViewDelta(vec);
}


void TGen::Engine::GameInputMapper::setPlayerController(TGen::Engine::PlayerController * controller) {
	playerController = controller;
}

