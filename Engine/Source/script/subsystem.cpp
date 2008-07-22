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
	newEvent->setMinCallInterval(TGen::lexical_cast<scalar>(properties.getProperty("callTime", "-1.0")));
	
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
		std::string fixedSource = container.getAlias(properties.getAttribute(0));
		if (fixedSource.empty())
			fixedSource = properties.getAttribute(0);
		
		int source = getRegisterId(fixedSource);
		
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
		TGen::Engine::Script::IfOperation * newIf = createIfOperation(type, 0, properties, container);
		ret = newIf;
	}
	else if (type == "else") {
		TGen::Engine::Script::IfOperation * lastIf = dynamic_cast<TGen::Engine::Script::IfOperation *>(lastCreatedOp);
		
		if (!lastIf)
			throw TGen::RuntimeException("Script::Subsystem::createOperation", "'else' should come directly after if!");
		
		TGen::Engine::Script::FrameOperation * newOp = NULL;
		
		if (properties.getNumAttributes() > 0 && properties.getAttribute(0) == "if") {
			newOp = createIfOperation(properties.getAttribute(0), 1, properties, container);
		}
		else {
			newOp = new TGen::Engine::Script::FrameOperation(&container);
		}
		
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
			if (properties.getAttribute(2)[0] == '[') {	// beginning of a call
				container.addOperation(createMovOperation("imov", TGen::lexical_cast<std::string>(registerNum), "r1", container));
				
				createCallOperation(properties.getAttribute(2), properties, 3, container);
			}
			else {
				ret = createMovOperation("mov", properties.getAttribute(2), registerName, container);
			}
		}
	}
	else if (type[0] == '[') {
		createCallOperation(type, properties, 0, container);
	}
	else if (type == "frame") {
		TGen::Engine::Script::FrameOperation * newOp = new TGen::Engine::Script::FrameOperation(&container);
		createOperations(*newOp, properties);
		
		ret = newOp;
	}
	else {
		if (properties.getAttribute(0) == "=") {
			if (properties.getAttribute(1)[0] == '[') {	// beginning of a call
				std::string regName = container.getAlias(properties.getName());
				if (regName.empty())
					regName = properties.getName();
				
				int regNum = getRegisterId(regName);
				if (regNum == -1)
					throw TGen::RuntimeException("Script::Subsystem::createOperation", "invalid register: " + properties.getName());
				
				TGen::Engine::Script::EventOperation * retMov = createMovOperation("imov", TGen::lexical_cast<std::string>(regNum), "r1", container);
				container.addOperation(retMov);	// should be in the container above frame

			
				createCallOperation(properties.getAttribute(1), properties, 2, container);
				
			//	exit(222);
			}
			else {
				ret = createMovOperation("mov", properties.getAttribute(1), properties.getName(), container);
			}
		}
	}
	
	if (ret)
		lastCreatedOp = ret;
	
	if (ret)
		container.addOperation(ret);
}

TGen::Engine::Script::IfOperation * TGen::Engine::Script::Subsystem::createIfOperation(const std::string & type, int attributeStart, const TGen::PropertyTree & properties, TGen::Engine::Script::EventOperation & container) {
	
	TGen::Engine::Script::IfOperation * newOp = new TGen::Engine::Script::IfOperation(&container);
	std::string compare;
	
	std::string firstPara = properties.getAttribute(0 + attributeStart);
	std::string secondPara;
	
	bool parenthesis = (firstPara[0] == '(');
	if (parenthesis)
		firstPara = firstPara.substr(1);
	
	if (parenthesis && properties.getNumAttributes() == 1 + attributeStart)
		firstPara = firstPara.substr(0, firstPara.size() - 1);
	
	std::string value = container.getAlias(firstPara);
	if (value.empty())
		value = firstPara;
	
	newOp->setRegister(getRegisterId(value));
	
	if (properties.getNumAttributes() > 1 + attributeStart) {
		compare = properties.getAttribute(1 + attributeStart);
		secondPara = properties.getAttribute(2 + attributeStart);
		
		if (parenthesis)
			secondPara = secondPara.substr(0, secondPara.size() - 1);
		
		newOp->setValue(TGen::lexical_cast<scalar>(secondPara));
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
	
	return newOp;
}


void TGen::Engine::Script::Subsystem::createCallOperation(const std::string & head, const TGen::PropertyTree & properties, int attributeStart, TGen::Engine::Script::EventOperation & container) {
	bool saveContext = true;
	
	TGen::Engine::Script::EventOperation * parentObject = &container;
	
	if (saveContext) {
		TGen::Engine::Script::FrameOperation * frame = new TGen::Engine::Script::FrameOperation(&container);
		frame->setSaveReturn(true);
		
		container.addOperation(frame);
		parentObject = frame;
	}
	
	// dig out the object and method, put into fixedObject/fixedMethod
	
	std::string object, method;
	object = head;
	
	if (properties.getNumAttributes() > attributeStart && object[object.size() - 1] != ':') {		// fails if there's no object
		method = properties.getAttribute(attributeStart);
		attributeStart++;
	}
	
	std::string fixedObject, fixedMethod;
	if ((object.find(":") != std::string::npos) || (object.find("]") != std::string::npos)) {
		fixedMethod = object.substr(1, object.size() - 2);
	}
	else {
		fixedObject = object.substr(1);
		fixedMethod = method.substr(0, method.size() - 1);
	}
	
	// create movs for parameters:
	
	if (properties.getNumAttributes() >= attributeStart) {		// we've got parameters
		for (int i = properties.getNumAttributes() - 1; i >= attributeStart; --i) {		// steps backwards, some register collisions are avoided
			std::string attribute = properties.getAttribute(i);
			
			if (attribute[attribute.size() - 1] == ']')		// the last parameter ends with a ]
				attribute = attribute.substr(0, attribute.size() - 1);
			
			std::string dest = "r" + TGen::lexical_cast<std::string>(i - attributeStart + 2);	// select register to put parameter in
			parentObject->addOperation(createMovOperation("mov", attribute, dest, *parentObject));		// create move operation in parent object (frame object)
		}		
	}
	

	// create main body of call:
	
	TGen::Engine::Script::CallOperation * newCall = new TGen::Engine::Script::CallOperation(parentObject);
	newCall->setShareContext(true);

	
	// set object to work on:
	
	if (fixedObject[0] == 'r') {
		newCall->setEvent(getRegisterId(fixedObject));
	}
	else if (fixedObject == "self") {
		newCall->setEvent(TGen::Engine::RegisterSelf);
	}
	else if (fixedObject == "?") {
		newCall->setEvent(TGen::Engine::RegisterSelf);		// call entity
		newCall->setTriggerMode(TGen::Engine::TriggerFirst);
	}
	else if (fixedObject == "*") {
		newCall->setEvent(TGen::Engine::RegisterSelf);
		newCall->setTriggerMode(TGen::Engine::TriggerAll);
	}
	else {
		newCall->setEvent(fixedObject);
	}
	
	// set method:
	if (!fixedMethod.empty())
		newCall->setOffset(symbols[fixedMethod]);		// look up offset/symbol for method

	parentObject->addOperation(newCall);
	
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

