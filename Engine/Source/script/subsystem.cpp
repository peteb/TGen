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
#include "script/moveop.h"
#include "script/callop.h"
#include "script/frameop.h"

using TGen::Engine::Script::Event;

TGen::Engine::Script::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs)
	: logs(logs)
	, lastCreatedOp(NULL)
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
	
	if (type == "inci" || type == "stoi" || type == "iinci" || type == "istoi" || type == "lodi" || type == "ilodi") {
		TGen::Engine::Script::ItemValueOperation * newOp = new TGen::Engine::Script::ItemValueOperation;
	
		newOp->setItem(properties.getProperty("item", ""));		
		newOp->setRegister(getRegisterId(properties.getAttribute(0)));
		newOp->setRelative(type == "inci" || type == "iinci");
		newOp->setIntOp(type == "iinci" || type == "istoi" || type == "ilodi");
		newOp->setLoad(type == "lodi");
		
		ret = newOp;
	}
	else if (type == "mad" || type == "imad") {
		TGen::Engine::Script::MadOperation * newOp = new TGen::Engine::Script::MadOperation;
		
		bool intMath = (type == "imad");
		int source = getRegisterId(properties.getAttribute(0));
		
		newOp->setSource(source);
		
		if (properties.getNumAttributes() > 1)
			newOp->setDest(getRegisterId(properties.getAttribute(1)));
		else
			newOp->setDest(source);
		
		if (intMath) {
			newOp->setIntTerm(TGen::lexical_cast<int>(properties.getProperty("add", "1")));
		}
		else {
			newOp->setFactor(TGen::lexical_cast<scalar>(properties.getProperty("mul", "1")));
			newOp->setTerm(TGen::lexical_cast<scalar>(properties.getProperty("add", "0")));
		}
		
		newOp->setIntMath(intMath);
		
		ret = newOp;
	}
	else if (type == "playSound") {
		TGen::Engine::Script::SoundOperation * newOp = new TGen::Engine::Script::SoundOperation;
		newOp->setSource(properties.getProperty("source", ""));
		newOp->setSound(properties.getAttribute(0));
		
		ret = newOp;
	}
	else if (type == "if" || type == "while") {
		TGen::Engine::Script::IfOperation * newOp = new TGen::Engine::Script::IfOperation;
		newOp->setRegister(getRegisterId(properties.getAttribute(0)));
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
		newOp->setLoop(type == "while");
		
		createOperations(*newOp, properties);
		
		ret = newOp;
	}
	else if (type == "else") {
		TGen::Engine::Script::IfOperation * lastIf = dynamic_cast<TGen::Engine::Script::IfOperation *>(lastCreatedOp);
		
		if (!lastIf)
			throw TGen::RuntimeException("Script::Subsystem::createOperation", "'else' should come directly after if!");
		
		TGen::Engine::Script::FrameOperation * newOp = new TGen::Engine::Script::FrameOperation;
		newOp->setSaveContext(false);
		newOp->setExecute(false);
		ret = newOp;
		
		createOperations(*newOp, properties);
		
		lastIf->setElseBlock(newOp);
	}
	else if (type == "mov" || type == "swap" || type == "imov" || type == "iswap") {
		TGen::Engine::Script::MoveOperation * newOp = new TGen::Engine::Script::MoveOperation;
		
		bool intOp = (type == "imov" || type == "iswap");
		int sourceId = getRegisterId(properties.getAttribute(0));

		if (sourceId != -1) {
			newOp->setSource(sourceId);
		}
		else {
			if (intOp)
				newOp->setSourceImm(TGen::lexical_cast<int>(properties.getAttribute(0)));
			else
				newOp->setSourceImm(TGen::lexical_cast<scalar>(properties.getAttribute(0)));
		}
		
		newOp->setDest(getRegisterId(properties.getAttribute(1)));
		newOp->setSwap(type == "swap" || type == "iswap");
					
		ret = newOp;
	}
	else if (type == "call" || type == "callns") {
		TGen::Engine::Script::CallOperation * newOp = new TGen::Engine::Script::CallOperation;
		
		newOp->setEvent(properties.getAttribute(0));
		newOp->setShareContext(type == "call");
		
		ret = newOp;
	}
	else if (type == "frame") {
		TGen::Engine::Script::FrameOperation * newOp = new TGen::Engine::Script::FrameOperation;
		
		createOperations(*newOp, properties);
		
		ret = newOp;
	}
	
	lastCreatedOp = ret;
	
	return ret;
}

int TGen::Engine::Script::Subsystem::getRegisterId(const std::string & desc) {
	if (desc[0] == 'r')
		return TGen::lexical_cast<int>(desc.substr(1));
	
	return -1;
}
