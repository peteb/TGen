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
#include "world.h"
#include "inputdevice.h"
#include "playercontroller.h"

TGen::Engine::GameInputMapper::GameInputMapper(TGen::Engine::PlayerController & controller)
	: world(NULL)
	, playerController(controller)
{
	
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

	int eventId = keyToEventID(id);
	
	if (state == TGen::Engine::StateDown)
		playerController.beginEvent(eventId);
	else if (state == TGen::Engine::StateUp)
		playerController.endEvent(eventId);	
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
	std::stringstream deviceName;
	deviceName << device.getName() << device.getId() << " (" << device.getDeviceName() << ")";

	if (world && vec.getMagnitude() < 100.0f) {
		world->getCamera("maincam")->setPosition(world->getCamera("maincam")->getLocalPosition() + TGen::Vector3(vec.x, 0.0f, -vec.y) * 0.5);
	//	reinterpret_cast<TGen::FPSCamera *>(world->getCamera("maincam"))->rotate(TGen::Vector2(vec) * 0.01f);
	}
	
	//rotate(const TGen::Vector2 & rot)
	std::cout << "*** vector move on " << deviceName.str() << " id: " << id << ": " << std::string(vec) << std::endl;
}

void TGen::Engine::GameInputMapper::setWorld(TGen::Engine::World * world) {
	this->world = world;
}

