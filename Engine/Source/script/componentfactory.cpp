/*
 *  componentfactory.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/18/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/componentfactory.h"
#include "script/event.h"
#include "script/madop.h"
#include "script/ifop.h"
#include "script/moveop.h"
#include "script/callop.h"
#include "script/frameop.h"
#include "script/eventrecipe.h"
#include "script/debugop.h"
#include "script/luacallop.h"

TGen::Engine::Script::ComponentFactory::ComponentFactory(TGen::Engine::Script::Subsystem & subsystem)
	: lastCreatedOp(NULL)
	, subsystem(subsystem)
{
}

TGen::Engine::ComponentRecipe * TGen::Engine::Script::ComponentFactory::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	TGen::Engine::Script::Event * newComponent = createComponent(name, entityName, properties);
	
	return new TGen::Engine::Script::EventRecipe(name, newComponent);	
}


TGen::Engine::Script::Event * TGen::Engine::Script::ComponentFactory::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	int symbolId = -1;
	bool hasParameterList = false;
	
	std::string fixedName = name;
	
	if (fixedName[fixedName.size() - 1] == ':') {
		fixedName = fixedName.substr(0, fixedName.size() - 1);
		hasParameterList = true;
	}
	
	
	int paraCount = 0;
	std::string mangledName = fixedName;
	std::vector<std::pair<std::string, std::string> > aliases;
	
	if (hasParameterList) {
		mangledName += ":";
		
		for (int i = 1; i < properties.getNumAttributes(); ++i, ++paraCount) {
			std::string attribute = properties.getAttribute(i);
			
			std::string parameterName = properties.getAttribute(i);
			std::string aliasName = properties.getAttribute(i);
			
			std::string::value_type pos = attribute.find(":");
			if (pos != std::string::npos) {
				parameterName = attribute.substr(0, pos);
				aliasName = attribute.substr(pos + 1);
			}
			
			std::string registerName = "r" + TGen::lexical_cast<std::string>(i + TGen::Engine::RegisterParameters - 1);
			
			aliases.push_back(std::make_pair(aliasName, registerName));
			
			if (i > 1 && !mangledName.empty())	
				mangledName += parameterName + ":";
		}
	}
	
	std::cout << "EVENTFUNC: " << mangledName << std::endl;
	
	if (fixedName == "initialize" || fixedName == "init")
		symbolId = -2;
	else if (fixedName == "dispatch")
		symbolId = -3;
	else if (properties.getName()[0] != '-')
		symbolId = TGen::Engine::getUniqueSymbol(mangledName);
	
	Event * newEvent = new Event(fixedName, symbolId);	
	
	for (int i = 0; i < aliases.size(); ++i)
		newEvent->addAlias(aliases[i].first, aliases[i].second);
	
	newEvent->setNumParameters(paraCount);
	newEvent->setMinCallInterval(TGen::lexical_cast<scalar>(properties.getProperty("callTime", "-1.0")));
	
	createOperations(*newEvent, properties);
	
	return newEvent;	
}




void TGen::Engine::Script::ComponentFactory::createOperations(TGen::Engine::Script::EventOperation & container, const TGen::PropertyTree & properties) {
	for (int i = 0; i < properties.getNumNodes(); ++i) {
		const TGen::PropertyTree & node = properties.getNode(i);
		
		createOperation(container, node);
	}	
}

void TGen::Engine::Script::ComponentFactory::createOperation(TGen::Engine::Script::EventOperation & container, const TGen::PropertyTree & properties) {
	TGen::Engine::Script::EventOperation * ret = NULL;
	const std::string & type = properties.getName();
	
	if (type == "mad" || type == "imad") {
		TGen::Engine::Script::MadOperation * newOp = new TGen::Engine::Script::MadOperation(&container);
		
		bool intMath = (type == "imad");
		
		std::string fixedSource = properties.getAttribute(0);
		std::string sourceOffset = "-1";
		
		{
			std::string::value_type pos = fixedSource.find("[");
			
			if (pos != std::string::npos) {
				sourceOffset = fixedSource.substr(pos + 1, fixedSource.find("]") - pos - 1);
				fixedSource = fixedSource.substr(0, pos);
			}
		}
		
		fixedSource = container.getAlias(fixedSource);
		if (fixedSource.empty())
			fixedSource = properties.getAttribute(0);
		
		int source = getRegisterId(fixedSource);
		
		newOp->setSource(source);
		newOp->setSourceOffset(TGen::lexical_cast<int>(sourceOffset));
		
		if (properties.getNumAttributes() > 1)
			newOp->setDest(getRegisterId(properties.getAttribute(1)));
		else {
			newOp->setDest(source);
			newOp->setDestOffset(TGen::lexical_cast<int>(sourceOffset));
		}
		
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
	else if (type == "debug") {
		TGen::Engine::Script::DebugOperation * newOp = new TGen::Engine::Script::DebugOperation(&container);
		
		if (properties.getNumAttributes() > 0) {
			if (properties.getAttribute(0) == "level")
				newOp->setDebugLevel(true);
		}
		
		ret = newOp;
	}
	else if (type == "luaCall") {
		ret = createLuaCallOperation(type, properties, container);
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
		int registerNum = container.getNumParameters() + TGen::Engine::RegisterParameters + container.getNumLocalVars() + TGen::Engine::ReservedParameters;
		std::string registerName = "r" + TGen::lexical_cast<std::string>(registerNum);
		
		container.setNumLocalVars(container.getNumLocalVars() + 1);		// crappy getters/setters
		container.addAlias(properties.getAttribute(0), registerName);
		
		// TODO: alla operationer ska lösa ut aliases
		// TODO: en funktion som kollar om ett värde är en scalar eller en int. en int innehåller ingen punkt, det gör scalar
		
		if (properties.getNumAttributes() > 2 && properties.getAttribute(1) == "=") {
			if (properties.getAttribute(2)[0] == '[') {	// beginning of a call
				std::cout << "RETURN TO " << properties.getAttribute(0) << " WHICH IS " << registerNum << std::endl;
				
				
				TGen::Engine::Script::EventOperation * callOperation = createCallOperation(properties.getAttribute(2), properties, 3, container);

				callOperation->insertOperation(0, createMovOperation("imov", TGen::lexical_cast<std::string>(registerNum), "r1", *callOperation));

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
				TGen::Engine::Script::EventOperation * callOperation = createCallOperation(properties.getAttribute(1), properties, 2, container);

				
				std::string regName = container.getAlias(properties.getName());
				if (regName.empty())
					regName = properties.getName();
				
				int regNum = getRegisterId(regName);
				if (regNum == -1)
					throw TGen::RuntimeException("Script::Subsystem::createOperation", "invalid register: " + properties.getName());
				
				TGen::Engine::Script::EventOperation * retMov = createMovOperation("imov", TGen::lexical_cast<std::string>(regNum), "r1", container);
				callOperation->insertOperation(0, retMov);
				
				
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

TGen::Engine::Script::IfOperation * TGen::Engine::Script::ComponentFactory::createIfOperation(const std::string & type, int attributeStart, const TGen::PropertyTree & properties, TGen::Engine::Script::EventOperation & container) {
	
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


TGen::Engine::Script::EventOperation * TGen::Engine::Script::ComponentFactory::createCallOperation(const std::string & head, const TGen::PropertyTree & properties, int attributeStart, TGen::Engine::Script::EventOperation & container) {
	bool saveContext = true;
	
	TGen::Engine::Script::EventOperation * parentObject = &container;
	
	if (saveContext) {
		TGen::Engine::Script::FrameOperation * frame = new TGen::Engine::Script::FrameOperation(&container);
		frame->setSaveReturn(true);
		
		container.addOperation(frame);
		parentObject = frame;
	}
	
	// dig out the object and method, put into fixedObject/fixedMethod
	
	std::string object, method, mangledMethodName;
	object = head;
	
	if (properties.getNumAttributes() > attributeStart && object[object.size() - 1] != ':') {		// fails if there's no object
		method = properties.getAttribute(attributeStart);
		attributeStart++;
	}
	
	std::string fixedObject, fixedMethod, fixedEntity;
	
	if ((object.find(":") != std::string::npos) || (object.find("]") != std::string::npos)) {
		fixedMethod = object.substr(1, object.size() - 1);
	}
	else {
		fixedObject = object.substr(1);
		fixedMethod = method.substr(0);
	}
	
	if (fixedObject.find(".") != std::string::npos) {	// we're addressing another entity, like "entity.?"
		std::string::value_type pos = fixedObject.find(".");
		
		fixedEntity = fixedObject.substr(0, pos);
		fixedObject = fixedObject.substr(pos + 1);
	}
	
	if (fixedMethod[fixedMethod.size() - 1] == ']')
		fixedMethod = fixedMethod.substr(0, fixedMethod.size() - 1);
	
	
	// resolve object name if it's a named register
	std::string aliased = container.getAlias(fixedObject);
	if (!aliased.empty())
		fixedObject = aliased;
	
	mangledMethodName = fixedMethod;
	
	// create mov for object: (into register 2)
	
	if (fixedObject[0] == '@') {
		parentObject->addOperation(createMovOperation("mov", fixedObject, "r2", *parentObject));
		fixedObject = "r2";
	}
	
	
	// create movs for parameters:
	
	int numParameters = 0;
	
	std::vector<std::pair<std::string, std::string> > parameters;
	
	if (properties.getNumAttributes() > attributeStart) {		// we've got parameters
		parameters.push_back(std::make_pair("", properties.getAttribute(attributeStart)));
		
		
		for (int i = attributeStart + 1; i < properties.getNumAttributes(); i += 2) {
			std::string paramName = properties.getAttribute(i);
			std::string paramValue = properties.getAttribute(i + 1);
			
			parameters.push_back(std::make_pair(paramName, paramValue));
			
			mangledMethodName += paramName;
		}
	}
	
	/*std::cout << "PARAMETERS:" << std::endl;
	 for (int i = 0; i < parameters.size(); ++i)
	 std::cout << parameters[i].first << " = " << parameters[i].second << std::endl;
	 */
	
	for (int i = parameters.size() - 1; i >= 0; --i) {
		std::string paramValue = parameters[i].second;
		
		if (paramValue[paramValue.size() - 1] == ']')
			paramValue = paramValue.substr(0, paramValue.size() - 1);
		
		
		std::string dest = "r" + TGen::lexical_cast<std::string>(i + TGen::Engine::RegisterParameters);	// select register to put parameter in
		
		parentObject->addOperation(createMovOperation("mov", paramValue, dest, *parentObject));		// create move operation in parent object (frame object)
		numParameters++;		
	}
	
	// create main body of call:
	
	TGen::Engine::Script::CallOperation * newCall = new TGen::Engine::Script::CallOperation(parentObject);
	newCall->setShareContext(true);
	newCall->setNumParameters(numParameters);
	
	// set entity:
	if (!fixedEntity.empty())
		newCall->setEntity(fixedEntity);
	
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
	
	std::cout << "MANGLED NAME: " << mangledMethodName << std::endl;
	
	// set method:
	if (!fixedMethod.empty())
		newCall->setOffset(TGen::Engine::getUniqueSymbol(mangledMethodName));		// look up offset/symbol for method
	
	parentObject->addOperation(newCall);	
	
	return parentObject;
}


TGen::Engine::Script::MoveOperation * TGen::Engine::Script::ComponentFactory::createMovOperation(const std::string & type, const std::string & source, const std::string & dest, TGen::Engine::Script::EventOperation & container) {
	TGen::Engine::Script::MoveOperation * newOp = new TGen::Engine::Script::MoveOperation(&container);
	
	bool intOp = (type == "imov" || type == "iswap");
	
	std::string fixedDest = dest;
	std::string fixedSource = source;
	std::string destOffset = "-1";
	std::string sourceOffset = "-1";
	
	if (source == "false")
		fixedSource = "0";
	else if (source == "true")
		fixedSource = "1";
	
	{
		std::string::value_type pos = fixedDest.find("[");
		
		if (pos != std::string::npos) {
			destOffset = fixedDest.substr(pos + 1, fixedDest.find("]") - pos - 1);
			fixedDest = fixedDest.substr(0, pos);
		}
	}
	
	{
		std::string::value_type pos = fixedSource.find("[");
		
		if (pos != std::string::npos) {
			sourceOffset = fixedSource.substr(pos + 1, fixedSource.find("]") - pos - 1);
			fixedSource = fixedSource.substr(0, pos);
		}
	}
	
	newOp->setDestOffset(TGen::lexical_cast<int>(destOffset));
	newOp->setSourceOffset(TGen::lexical_cast<int>(sourceOffset));
	
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
				newOp->setSourceImm(TGen::Engine::getUniqueSymbol(fixedSource));
			}
		}
	}
	
	newOp->setDest(getRegisterId(fixedDest));
	newOp->setSwap(type == "swap" || type == "iswap");
	newOp->setDerefDest(type == "ret");
	
	return newOp;	
}


TGen::Engine::Script::LuaCallOperation * TGen::Engine::Script::ComponentFactory::createLuaCallOperation(const std::string & type, 
																																		const TGen::PropertyTree & properties, 
																																		TGen::Engine::Script::EventOperation & container) 
{
	TGen::Engine::Script::LuaCallOperation * newCall = new TGen::Engine::Script::LuaCallOperation(&container, subsystem);
	newCall->setFunctionName(properties.getAttribute(0));
	
	for (int i = 0; i < properties.getNumNodes(); ++i) {
		const TGen::PropertyTree & node = properties.getNode(i);
		
		if (node.getName() == "return") {
			newCall->addReturnToRegister(getRegisterId(container.getAlias(node.getAttribute(0))));
		}
		else if (node.getName() == "param") {
			newCall->addParameterRegister(getRegisterId(container.getAlias(node.getAttribute(0))));
		}
	}
	
	return newCall;
}


int TGen::Engine::Script::ComponentFactory::getRegisterId(const std::string & desc) {
	if (desc[0] == 'r')
		return TGen::lexical_cast<int>(desc.substr(1));
	
	return -1;
}


