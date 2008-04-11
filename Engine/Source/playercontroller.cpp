/*
 *  playercontroller.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/11/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "playercontroller.h"

TGen::Engine::PlayerController::PlayerController() {
	for (int i = 0; i < 20; ++i)
		activeEvents[i] = 0;	
}


void TGen::Engine::PlayerController::beginEvent(int id) {
	if (id >= 0 && id < 20)
		activeEvents[id] |= EventTriggered;
}

void TGen::Engine::PlayerController::endEvent(int id) {
	if (id >= 0 && id < 20) {
		if (!(activeEvents[id] & EventRead))
			activeEvents[id] |= EventKilled;
		else
			activeEvents[id] = 0;
	}
}

void TGen::Engine::PlayerController::update(scalar dt) {
	if (checkEvent(EventForward)) {
		std::cout << "FORWARD " << dt << std::endl;
		
	}
}

bool TGen::Engine::PlayerController::checkEvent(int id) {
	if (id < 0 || id >= 20)
		return false;
	
	if (activeEvents[id] & EventTriggered) {
		if (activeEvents[id] & EventKilled)
			activeEvents[id] = 0;
		else if (!(activeEvents[id] & EventRead))
			activeEvents[id] |= EventRead;
		
		return true;
	}
	
	return false;
}