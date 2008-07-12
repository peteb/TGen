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

using TGen::Engine::Script::Event;

TGen::Engine::Script::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs)
	: logs(logs)
{
	
}

TGen::Engine::Script::Subsystem::~Subsystem() {
	
}


TGen::Engine::Component * TGen::Engine::Script::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	Event * newEvent = new Event(name);
	
	std::cout << "SCRIPTEVENT " << name << " - " << entityName << std::endl;
	
	//exit(1);
	for (int i = 0; i < properties.getNumNodes(); ++i) {
		const TGen::PropertyTree & node = properties.getNode(i);
		
		TGen::Engine::Script::EventOperation * newOp = createOperation(node);
		
		if (newOp)
			newEvent->addOperation(newOp);
	}
	
	//exit(1);
	
	return newEvent;
}

TGen::Engine::ComponentRecipe * TGen::Engine::Script::Subsystem::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	
	return NULL;
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
	
	
	return ret;
}
