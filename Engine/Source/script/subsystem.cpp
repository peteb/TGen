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
	bool hasParameterList = false;
	
	std::string fixedName = name;
	
	if (fixedName[fixedName.size() - 1] == ':') {
		fixedName = fixedName.substr(0, fixedName.size() - 1);
		hasParameterList = true;
	}
	
	if (fixedName == "initialize")
		symbolId = -2;
	else if (fixedName == "dispatch")
		symbolId = -3;
	else if (properties.getName()[0] != '-')
		symbolId = symbols[fixedName];
	
	Event * newEvent = new Event(fixedName, symbolId);
	
	int paraCount = 0;
	
	if (hasParameterList) {
		for (int i = 1; i < properties.getNumAttributes(); ++i, ++paraCount) {
			std::string registerName = "r" + TGen::lexical_cast<std::string>(i + 1);
			
			newEvent->addAlias(properties.getAttribute(i), registerName);
		}
	}
	
	newEvent->setNumParameters(paraCount);
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
		TGen::Engine::Script::MadOperation * newOp = new TGen::Engine::Script::MadOperation(&container);
		
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
		TGen::Engine::Script::IfOperation * newOp = new TGen::Engine::Script::IfOperation(&container);
		std::string compare;
		
		std::string value = container.getAlias(properties.getAttribute(0));
		if (value.empty())
			value = properties.getAttribute(0);
		
		newOp->setRegister(getRegisterId(value));
		
		if (properties.getNumAttributes() > 1) {
			newOp->setValue(TGen::lexical_cast<scalar>(properties.getAttribute(2)));
			compare = properties.getAttribute(1);
		}
		else {
			newOp->setValue(0);
			newOp->setIntOp(true);
			compare = "!=";
		}
		
		TGen::Engine::Script::CompareType compareType;

		if (compare == "==")
			compareType = TGen::Engine::Script::CompareEquals;
		else if (compare == "!=")
			compareType = TGen::Engine::Script::CompareNotEquals;
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
		
		TGen::Engine::Script::FrameOperation * newOp = new TGen::Engine::Script::FrameOperation(&container);
		newOp->setSaveContext(false);
		newOp->setExecute(false);
		ret = newOp;
		
		createOperations(*newOp, properties);
		
		lastIf->setElseBlock(newOp);
	}
	else if (type == "mov" || type == "swap" || type == "imov" || type == "iswap" || type == "return" || type == "returni") {
		if (type == "return" || type == "returni") {
			TGen::Engine::Script::MoveOperation * newOp = createMovOperation((type == "return" ? "mov" : "imov"), properties.getAttribute(0), "r1", container);
			newOp->setDerefDest(true);
			ret = newOp;
		}
		else {
			ret = createMovOperation(type, properties.getAttribute(0), properties.getAttribute(1), container);
		}
	}
	else if (type == "var") {
		int registerNum = container.getNumParameters() + 2 + container.getNumLocalVars();
		std::string registerName = "r" + TGen::lexical_cast<std::string>(registerNum);
		
		container.setNumLocalVars(container.getNumLocalVars() + 1);		// crappy getters/setters
		container.addAlias(properties.getAttribute(0), registerName);
		
		// TODO: alla operationer ska lösa ut aliases
		// TODO: en funktion som kollar om ett värde är en scalar eller en int. en int innehåller ingen punkt, det gör scalar
		
		if (properties.getNumAttributes() > 2 && properties.getAttribute(1) == "=") {
			TGen::Engine::Script::MoveOperation * newOp = createMovOperation("mov", properties.getAttribute(2), registerName, container);
			ret = newOp;
		}
	}
	else if (type[0] == '[' || (properties.getNumAttributes() > 2 && properties.getAttribute(1)[0] == '[')) {
		bool retValue = (properties.getNumAttributes() > 2 && properties.getAttribute(0) == "=");
		bool saveContext = true;
		int attributeOffset = 0;
		
		std::string object, method;
		int paramStart = 0;
		
		if (retValue) {
			paramStart = 2;
			
			std::string regName = container.getAlias(properties.getName());
			if (regName.empty())
				regName = properties.getName();
			
			int regNum = getRegisterId(regName);
			if (regNum == -1)
				throw TGen::RuntimeException("Script::Subsystem::createOperation", "invalid register: " + properties.getName());
			
			TGen::Engine::Script::EventOperation * retMov = createMovOperation("mov", TGen::lexical_cast<std::string>(regNum), "r1", container);
			container.addOperation(retMov);
			
			object = properties.getAttribute(1);
			method = properties.getAttribute(2);
		}
		else {
			object = properties.getName();
			method = properties.getAttribute(0);
		}
		
		TGen::Engine::Script::CallOperation * newOp = new TGen::Engine::Script::CallOperation(&container);
		
				
		std::string objectPart, methodPart;
		
		if ((object.find(":") != std::string::npos) || (object.find("]") != std::string::npos)) {
			methodPart = object.substr(1, object.size() - 2);
		}
		else {
			objectPart = object.substr(1);
			methodPart = method.substr(0, method.size() - 1);
						
		}
		
		/*int pointPos = properties.getAttribute(attributeOffset).find(".");
		
		if (pointPos != std::string::npos) {
			objectPart = properties.getAttribute(attributeOffset).substr(0, pointPos);
			methodPart = properties.getAttribute(attributeOffset).substr(pointPos + 1);
		}
		else {
			objectPart = properties.getAttribute(attributeOffset);
		}*/
		
		std::cout << "OBJECT: " << objectPart << " METHOD: " << methodPart << std::endl;
		
		if (objectPart[0] == 'r') {
			newOp->setEvent(getRegisterId(objectPart));
		}
		else if (objectPart == "self") {
			newOp->setEvent(TGen::Engine::RegisterSelf);
		}
		else if (objectPart == "?") {
			newOp->setEvent(TGen::Engine::RegisterSelf);
			newOp->setTriggerMode(TGen::Engine::TriggerFirst);
		}
		else if (objectPart == "*") {
			newOp->setEvent(TGen::Engine::RegisterSelf);
			newOp->setTriggerMode(TGen::Engine::TriggerAll);
		}		
		else {
			newOp->setEvent(objectPart);
		}
		
		if (!methodPart.empty())
			newOp->setOffset(symbols[methodPart]);
		
		newOp->setShareContext(true); //type == "call");
		
		TGen::Engine::Script::EventOperation * parentObject = &container;
		
		if (saveContext) {
			TGen::Engine::Script::FrameOperation * frame = new TGen::Engine::Script::FrameOperation(&container);
			frame->setSaveReturn(true);
			
			parentObject = frame;
		}
		
		if (properties.getNumAttributes() > 1 + paramStart) {	// we've got parameters
			for (int i = properties.getNumAttributes() - 1; i >=  1 + paramStart; --i) {
				std::string attribute = properties.getAttribute(i);
				
				if (attribute[attribute.size() - 1] == ']')
					attribute = attribute.substr(0, attribute.size() -1 );
				
				std::string dest;
				dest = "r" + TGen::lexical_cast<std::string>(i + 1 - paramStart);
				
				if (dest != attribute) {
					TGen::Engine::Script::EventOperation * paramMov = createMovOperation("mov", attribute, dest, container);
					if (!paramMov)
						throw TGen::RuntimeException("Script::Subsystem::createOperation", "failed to create mov for parameter");
				
					container.addOperation(paramMov);
				}
			}
		}
		
		if (saveContext) {
			parentObject->addOperation(newOp);
			ret = parentObject;
		}
		else {
			ret = newOp;
		}
	}
	else if (type == "frame") {
		TGen::Engine::Script::FrameOperation * newOp = new TGen::Engine::Script::FrameOperation(&container);
		
		createOperations(*newOp, properties);
		
		ret = newOp;
	}
	else {
		if (properties.getAttribute(0) == "=") {
			ret = createMovOperation("mov", properties.getAttribute(1), properties.getName(), container);
		}
	}
	
	lastCreatedOp = ret;
	
	if (ret)
		container.addOperation(ret);
}


TGen::Engine::Script::MoveOperation * TGen::Engine::Script::Subsystem::createMovOperation(const std::string & type, const std::string & source, const std::string & dest, TGen::Engine::Script::EventOperation & container) {
	TGen::Engine::Script::MoveOperation * newOp = new TGen::Engine::Script::MoveOperation(&container);

	bool intOp = (type == "imov" || type == "iswap");

	std::string fixedDest = dest;
	std::string fixedSource = source;

	
	std::string aliasDest = container.getAlias(fixedDest);
	if (!aliasDest.empty())
		fixedDest = aliasDest;
	
	std::string aliasSource;
	bool isResourceRef = false;
	
	if (fixedSource[0] != '@') {
		aliasSource = container.getAlias(fixedSource);
		if (!aliasSource.empty())
			fixedSource = aliasSource;
	}	
	else {
		fixedSource = fixedSource.substr(1);
		isResourceRef = true;
	}
	
	int sourceId = getRegisterId(fixedSource);
	
	if (sourceId != -1) {
		newOp->setSource(sourceId);
	}
	else {
		if (fixedSource == "self") {
			newOp->setSource(TGen::Engine::RegisterSelf);
		}
		else if (isResourceRef) {
			newOp->setResourceName(fixedSource);
		}
		else {
			try {
				if (intOp)
					newOp->setSourceImm(TGen::lexical_cast<int>(fixedSource));
				else
					newOp->setSourceImm(TGen::lexical_cast<scalar>(fixedSource));
			}
			catch (const std::bad_cast & e) {
				newOp->setSourceImm(symbols[fixedSource]);
			}
		}
	}
	
	newOp->setDest(getRegisterId(fixedDest));
	newOp->setSwap(type == "swap" || type == "iswap");
	newOp->setDerefDest(type == "ret");
	
	return newOp;	
}


int TGen::Engine::Script::Subsystem::getRegisterId(const std::string & desc) {
	if (desc[0] == 'r')
		return TGen::lexical_cast<int>(desc.substr(1));
	
	return -1;
}

