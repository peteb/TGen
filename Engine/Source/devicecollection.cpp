/*
 *  devicecollection.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "devicecollection.h"
#include "inputdevice.h"
#include "textoutput.h"

TGen::Engine::DeviceCollection::DeviceCollection() {
	
}

TGen::Engine::DeviceCollection::~DeviceCollection() {
	
}

void TGen::Engine::DeviceCollection::addDevice(TGen::Engine::InputDevice * device) {
	devices.push_back(device);
}

void TGen::Engine::DeviceCollection::dispatchEvents(TGen::Engine::InputEventResponder & responder) {
	for (int i = 0; i < devices.size(); ++i)
		devices[i]->dispatchEvents(responder);
}

void TGen::Engine::DeviceCollection::printDevices(TGen::Engine::TextOutputer & out) {
	for (int i = 0; i < devices.size(); ++i) {
		std::stringstream ss;
		ss << devices[i]->getName() << devices[i]->getId() << " (" << devices[i]->getDeviceName() << "): ";
		ss << devices[i]->getControls().numAxes << " axes, ";
		ss << devices[i]->getControls().numButtons << " buttons, ";
		ss << devices[i]->getControls().numBalls << " balls, ";
		ss << devices[i]->getControls().numHats << " hats";
		
		out.outputText(ss.str(), 0);
	}
}

