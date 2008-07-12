/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/subsystem.h"
#include "script/event.h"
#include "script/itemvalueop.h"
#include "script/madop.h"
#include "script/soundop.h"
#include "script/ifop.h"

using TGen::Engine::Script::Event;

TGen::Engine::Script::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs)
	: logs(logs)
{
	
}

TGen::Engine::Script::Subsystem::~Subsystem() {
	
}


TGen::Engine::Component * TGen::Engine::Script::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	Event * newEvent = new Event(name);
	
	createOperations(*newEvent, properties);
	
	return newEvent;
}

TGen::Engine::ComponentRecipe * TGen::Engine::Script::Subsystem::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	
	return NULL;
}

void TGen::Engine::Script::Subsystem::createOperations(TGen::Engine::Script::EventOperation & container, const TGen::PropertyTree & properties) {
	for (int i = 0; i < properties.getNumNodes(); ++i) {
		const TGen::PropertyTree & node = properties.getNode(i);
		
		TGen::Engine::Script::EventOperation * newOp = createOperation(node);
		
		if (newOp)
			container.addOperation(newOp);
	}	
}

TGen::Engine::Script::EventOperation * TGen::Engine::Script::Subsystem::createOperation(const TGen::PropertyTree & properties) {
	TGen::Engine::Script::EventOperation * ret = NULL;
	const std::string & type = properties.getName();
	
	if (type == "addItemValue") {
		TGen::Engine::Script::ItemValueOperation * newOp = new TGen::Engine::Script::ItemValueOperation;
	
		newOp->setItem(properties.getProperty("item", ""));		
		newOp->setParameter(TGen::lexical_cast<int>(properties.getProperty("param", "0")));
		
		ret = newOp;
	}
	else if (type == "mad") {
		TGen::Engine::Script::MadOperation * newOp = new TGen::Engine::Script::MadOperation;
		
		newOp->setParameter(TGen::lexical_cast<int>(properties.getProperty("param", "0")));
		newOp->setFactor(TGen::lexical_cast<scalar>(properties.getProperty("mult", "1")));
		newOp->setTerm(TGen::lexical_cast<scalar>(properties.getProperty("add", "0")));
		
		ret = newOp;
	}
	else if (type == "playSound") {
		TGen::Engine::Script::SoundOperation * newOp = new TGen::Engine::Script::SoundOperation;
		newOp->setSource(properties.getProperty("source", ""));
		newOp->setSound(properties.getProperty("sound", ""));
		
		ret = newOp;
	}
	else if (type == "if") {
		TGen::Engine::Script::IfOperation * newOp = new TGen::Engine::Script::IfOperation;
		newOp->setParam(TGen::lexical_cast<int>(properties.getAttribute(0)));
		newOp->setValue(TGen::lexical_cast<scalar>(properties.getAttribute(2)));
		
		TGen::Engine::Script::CompareType compareType;
		const std::string & compare = properties.getAttribute(1);
		
		if (compare == "==")
			compareType = TGen::Engine::Script::CompareEquals;
		else if (compare == ">")
			compareType = TGen::Engine::Script::CompareGreaterThan;
		else if (compare == "<")
			compareType = TGen::Engine::Script::CompareLessThan;
		else if (compare == ">=")
			compareType = TGen::Engine::Script::CompareGreaterThanOrEqual;
		else if (compare == "<=")
			compareType = TGen::Engine::Script::CompareLessThanOrEqual;
		
		newOp->setType(compareType);
		
		createOperations(*newOp, properties);
		
		ret = newOp;
	}
	
	return ret;
}
