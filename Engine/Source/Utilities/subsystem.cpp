/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "utilities/subsystem.h"
#include "utilities/timer.h"
#include "utilities/timer_script.h"
#include "utilities/objectreference.h"
#include "utilities/prototypereference.h"

TGen::Engine::Utilities::Subsystem::Subsystem() {
	
}

TGen::Engine::Utilities::Subsystem::~Subsystem() {
	
}

TGen::Engine::Component * TGen::Engine::Utilities::Subsystem::createComponent(const std::string & name, TGen::Engine::Entity & entity, const TGen::PropertyTree & properties) {
	std::string type = properties.getName();
	
	TGen::Engine::Component * ret = NULL;
	
	if (type == "timer") {
		Timer * newTimer = new TGen::Engine::Utilities::Timer(name);
		newTimer->setEnabled(TGen::lexical_cast<bool>(properties.getProperty("enabled", "true")));
		newTimer->setInterval(TGen::lexical_cast<scalar>(properties.getProperty("interval", "1.0")));
		newTimer->setOffsetTime(TGen::lexical_cast<scalar>(properties.getProperty("start", "0.0")));
		
		
		TGen::Engine::Utilities::TimerScript * scriptInterface = new TGen::Engine::Utilities::TimerScript(name, *newTimer, entity.getScriptInterface());
		newTimer->setScriptInterface(scriptInterface);
		timers.push_back(newTimer);

		ret = newTimer;
	}
	else if (type == "objectRef") {
		TGen::Engine::Utilities::ObjectReference * ref = new TGen::Engine::Utilities::ObjectReference(name);
		ref->setObjectName(properties.getAttribute(1));

		ret = ref;
	}
	else if (type == "prototypeRef") {
		TGen::Engine::Utilities::PrototypeReference * newRef = new TGen::Engine::Utilities::PrototypeReference(name);
		newRef->setPrototypeName(properties.getAttribute(1));
		ret = newRef;
	}
	else {
		throw TGen::RuntimeException("Utilities::Subsystem::createComponent", "invalid type: " + type);
	}
	
	return ret;
}

void TGen::Engine::Utilities::Subsystem::update(scalar dt) {
	for (int i = 0; i < timers.size(); ++i) {
		timers[i]->update(dt);
	}	
}
