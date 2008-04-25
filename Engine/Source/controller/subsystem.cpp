/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/15/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "controller/subsystem.h"
#include "playercontroller.h"

TGen::Engine::Controller::Subsystem::Subsystem() {
	
}

TGen::Engine::Controller::Subsystem::~Subsystem() {
	for (ControllerMap::iterator iter = controllers.begin(); iter != controllers.end(); ++iter)
		delete iter->second;
}

TGen::Engine::Component * TGen::Engine::Controller::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	TGen::Engine::PlayerController * newController = new TGen::Engine::PlayerController(name, TGen::lexical_cast<scalar>(properties.getProperty("speed", "1.0")));
	
	std::string type = properties.getProperty("type", "none");
	newController->addCamera("headcam", properties.getProperty("camera", "sceneCamera"));
	
	controllers.insert(ControllerMap::value_type(entityName, newController));
	
	return newController;
}

TGen::Engine::PlayerController * TGen::Engine::Controller::Subsystem::getController(const std::string & name) {
	ControllerMap::iterator iter = controllers.find(name);
	if (iter == controllers.end())
		throw TGen::RuntimeException("Controller::Subsystem::getController", "no controller called '" + name + "' defined!");
	
	return iter->second;
}

void TGen::Engine::Controller::Subsystem::update(scalar dt) {
	for (ControllerMap::iterator iter = controllers.begin(); iter != controllers.end(); ++iter) {
		iter->second->update(dt);
	}
}

// TODO: playercontroller baseclass, GhostController...    kunna välja om man vill ha fysik eller att den arbetar på sceneNode
// TODO: snurra scenenodes med musen på samma sätt som en FPS-kamera.
