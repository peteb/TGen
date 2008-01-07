/*
 *  inputdevice.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 1/7/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "inputdevice.h"

TGen::Engine::InputDevice::InputDevice(const std::string & name, int id)
	: name(name)
	, id(id)
{
}

TGen::Engine::InputDevice::~InputDevice() {

}

TGen::Engine::DeviceControls::DeviceControls() 
	: numAxes(0)
	, numButtons(0)
	, numBalls(0)
	, numHats(0)
{
}

const TGen::Engine::DeviceControls & TGen::Engine::InputDevice::getControls() const {
	return controls;
}

std::string TGen::Engine::InputDevice::getName() const {
	return name;
}

int TGen::Engine::InputDevice::getId() const {
	return id;
}

