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

TGen::SymbolTable TGen::Engine::Script::Subsystem::symbols;

TGen::Engine::Script::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs)
	: logs(logs)
	, lastCreatedOp(NULL)
{
	
}

TGen::Engine::Script::Subsystem::~Subsystem() {
	
}


TGen::Engine::Component * TGen::Engine::Script::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	int symbolId = -1;

	if (name == "initialize")
		symbolId = -2;
	else if (name == "dispatch")
		symbolId = -3;
	else if (properties.getName()[0] != '-')
		symbolId = symbols[name];
	
	Event * newEvent = new Event(name, symbolId);
	createOperations(*newEvent, properties);
	
	return newEvent;
}

TGen::Engine::ComponentRecipe * TGen::Engine::Script::Subsystem::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	
	return NULL;
}

void TGen::Engine::Script::Subsystem::createOperations(TGen::Engine::Script::EventOperation & container, const TGen::PropertyTree & properties) {
	for (int i = 0; i < properties.getNumNodes(); ++i) {
		const TGen::PropertyTree & node = properties.getNode(i);
		
		createOperation(container, node);
	}	
}

void TGen::Engine::Script::Subsystem::createOperation(TGen::Engine::Script::EventOperation & container, const TGen::PropertyTree & properties) {
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
		
		int termRegister = getRegisterId(properties.getProperty("add", "1"));
		int factorRegister = getRegisterId(properties.getProperty("mul", "0"));
		
		newOp->setTermRegister(termRegister);
		newOp->setFactorRegister(factorRegister);
		
		if (termRegister == -1) {
			if (intMath)
				newOp->setIntTerm(TGen::lexical_cast<int>(properties.getProperty("add", "1")));
			else
				newOp->setTerm(TGen::lexical_cast<scalar>(properties.getProperty("add", "0")));
		}
		
		if (factorRegister == -1) {
			if (intMath)
				newOp->setIntFactor(TGen::lexical_cast<int>(properties.getProperty("mul", "1")));
			else
				newOp->setFactor(TGen::lexical_cast<scalar>(properties.getProperty("mul", "1")));
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
	else if (type == "mov" || type == "swap" || type == "imov" || type == "iswap" || type == "ret") {
		if (type == "ret") {
			TGen::Engine::Script::MoveOperation * newOp = createMovOperation("imov", properties.getAttribute(0), "r1");
			newOp->setDerefDest(true);
			ret = newOp;
		}
		else {
			ret = createMovOperation(type, properties.getAttribute(0), properties.getAttribute(1));
		}
	}
	else if (type == "call" || type == "callns" || (properties.getNumAttributes() > 2 && properties.getAttribute(1) == "call" || properties.getAttribute(1) == "callns")) {
		bool retValue = (properties.getNumAttributes() > 2 && properties.getAttribute(0) == "=");
		int attributeOffset = 0;
		
		if (retValue) {
			attributeOffset = 2;
			
			int regNum = getRegisterId(properties.getName());
			if (regNum == -1)
				throw TGen::RuntimeException("Script::Subsystem::createOperation", "invalid register");
			
			TGen::Engine::Script::EventOperation * retMov = createMovOperation("imov", TGen::lexical_cast<std::string>(regNum), "r1");
			container.addOperation(retMov);
		}
		
		TGen::Engine::Script::CallOperation * newOp = new TGen::Engine::Script::CallOperation;
		
		
		
		std::string objectPart, methodPart;
		int pointPos = properties.getAttribute(attributeOffset).find(".");
		
		if (pointPos != std::string::npos) {
			objectPart = properties.getAttribute(attributeOffset).substr(0, pointPos);
			methodPart = properties.getAttribute(attributeOffset).substr(pointPos + 1);
		}
		else {
			objectPart = properties.getAttribute(attributeOffset);
		}
		
		if (objectPart[0] == 'r') {
			newOp->setEvent(getRegisterId(objectPart));
		}
		else if (objectPart == "self") {
			newOp->setEvent(TGen::Engine::RegisterSelf);
		}
		else {
			newOp->setEvent(objectPart);
		}
		
		if (!methodPart.empty())
			newOp->setOffset(symbols[methodPart]);
		
		newOp->setShareContext(type == "call");
		
		
		if (properties.getNumAttributes() > 1 + attributeOffset) {	// we've got parameters
			for (int i = 1 + attributeOffset; i < properties.getNumAttributes(); ++i) {
				std::cout << properties.getAttribute(i) << std::endl;
				std::string dest;
				dest = "r" + TGen::lexical_cast<std::string>(i + 1 - attributeOffset);
				
				TGen::Engine::Script::EventOperation * paramMov = createMovOperation("mov", properties.getAttribute(i), dest);
				if (!paramMov)
					throw TGen::RuntimeException("Script::Subsystem::createOperation", "failed to create mov for parameter");
				
				container.addOperation(paramMov);
			}
		}
		
		ret = newOp;
	}
	else if (type == "frame") {
		TGen::Engine::Script::FrameOperation * newOp = new TGen::Engine::Script::FrameOperation;
		
		createOperations(*newOp, properties);
		
		ret = newOp;
	}
	else {
		if (properties.getAttribute(0) == "=") {
			ret = createMovOperation("mov", properties.getAttribute(1), properties.getName());
		}
	}
	
	lastCreatedOp = ret;
	
	if (ret)
		container.addOperation(ret);
}

TGen::Engine::Script::MoveOperation * TGen::Engine::Script::Subsystem::createMovOperation(const std::string & type, const std::string & source, const std::string & dest) {
	TGen::Engine::Script::MoveOperation * newOp = new TGen::Engine::Script::MoveOperation;
	
	bool intOp = (type == "imov" || type == "iswap");
	int sourceId = getRegisterId(source);
	
	if (sourceId != -1) {
		newOp->setSource(sourceId);
	}
	else {
		if (source[0] == '$') {
			newOp->setSourceImm(symbols[source.substr(1)]);
		}
		else if (source == "self") {
			newOp->setSource(TGen::Engine::RegisterSelf);
		}
		else {
			if (intOp)
				newOp->setSourceImm(TGen::lexical_cast<int>(source));
			else
				newOp->setSourceImm(TGen::lexical_cast<scalar>(source));
		}
	}
	
	newOp->setDest(getRegisterId(dest));
	newOp->setSwap(type == "swap" || type == "iswap");
	newOp->setDerefDest(type == "ret");
	
	return newOp;	
}


int TGen::Engine::Script::Subsystem::getRegisterId(const std::string & desc) {
	if (desc[0] == 'r')
		return TGen::lexical_cast<int>(desc.substr(1));
	
	return -1;
}

