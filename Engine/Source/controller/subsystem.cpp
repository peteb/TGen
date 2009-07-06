/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/15/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "controller/subsystem.h"
#include "controller/firstpersoncontroller.h"
#include "controller/arcball.h"
#include "controller/interpolator.h"
#include "controller/interpolatorscript.h"

#include "playercontroller.h"

TGen::Engine::Controller::Subsystem::Subsystem() {
	
}


TGen::Engine::Controller::Subsystem::~Subsystem() {
	
}


void TGen::Engine::Controller::Subsystem::update(scalar dt) {
	for (ControllerMap::iterator iter = controllers.begin(); iter != controllers.end(); ++iter) {
		iter->second->update(dt);
	}
	
	for (int i = 0; i < interpolators.size(); ++i) {
		interpolators[i]->update(dt);
	}	
}


TGen::Engine::Component * TGen::Engine::Controller::Subsystem::createComponent(const std::string & name, TGen::Engine::Entity & entity, const TGen::PropertyTree & properties) {
	TGen::Engine::PlayerController * ret = NULL;
	
	std::string type = properties.getProperty("type", "none");
	
	if (type == "firstperson") {
		// TODO: spräng ut alla ctor-params till metoder

		std::auto_ptr<TGen::Engine::Controller::FirstPerson> newController(new TGen::Engine::Controller::FirstPerson(name,
																						  TGen::lexical_cast<scalar>(properties.getProperty("deltaPlane", "1.0")),
																						  TGen::lexical_cast<scalar>(properties.getProperty("jumpForce", "15000")),
																						  TGen::lexical_cast<scalar>(properties.getProperty("jumpTime", "0.3")) ));
		
		newController->setUsePhysics(TGen::lexical_cast<bool>(properties.getProperty("usePhysics", "false")));
		newController->setAirControl(TGen::lexical_cast<scalar>(properties.getProperty("airControl", "0.7")));
		newController->setView(properties.getProperty("view", "sceneNode"));
		newController->setControl(properties.getProperty("control", "sceneNode"));
		
		//newController->addCamera("headcam", properties.getProperty("camera", "sceneCamera"));
		newController->setWeaponLink(properties.getProperty("weapon", ""));
		newController->setEquipment(properties.getProperty("equipment", ""));
		
		ret = newController.release();
	}
	else if (type == "arcball") {
		TGen::Engine::Controller::Arcball * newArcball = new TGen::Engine::Controller::Arcball(name);
		
		newArcball->setControl(properties.getProperty("control", "sceneNode"));
		//newArcball->addCamera("headcam", properties.getProperty("camera", "sceneCamera"));
		
		ret = newArcball;
	}
	else if (properties.getName() == "interpolator") {
		TGen::auto_ptr<Interpolator> newInterpolator = new Interpolator(name);
		
		newInterpolator->setPositionDelegate(properties.getProperty("link", ""));
		newInterpolator->setSpeed(TGen::lexical_cast<scalar>(properties.getProperty("speed", "1.0")));
		
		TGen::Engine::Controller::InterpolatorScript * scriptInterface = new TGen::Engine::Controller::InterpolatorScript(name, *newInterpolator.get(), entity.getScriptInterface());
		newInterpolator->setScriptInterface(scriptInterface);
		
		
		TGen::PropertyTree::PropertyMap::const_iterator iter = properties.getNode("keypoints").getProperties().begin();
		for (; iter != properties.getNode("keypoints").getProperties().end(); ++iter) {
			newInterpolator->addKeypoint(TGen::Vector3::Parse(iter->second));
		}
		
		interpolators.push_back(newInterpolator.get());
		return newInterpolator.release();
	}
	else {
		throw TGen::RuntimeException("Controller::Subsystem::createComponent", "invalid controller type: '" + type + "'");
	}
	
	ret->setCamera(properties.getProperty("camera", ""));
	
	
	controllers.insert(std::make_pair(entity.getName(), ret));
	
	return ret;
}


TGen::Engine::PlayerController * TGen::Engine::Controller::Subsystem::getController(const std::string & name) {
	ControllerMap::iterator iter = controllers.find(name);
	if (iter == controllers.end())
		throw TGen::RuntimeException("Controller::Subsystem::getController", "no controller called '" + name + "' defined!");
	
	return iter->second;
}

