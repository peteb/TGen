/*
 *  frameop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/frameop.h"

TGen::Engine::Script::FrameOperation::FrameOperation(TGen::Engine::Script::EventOperation * parent) 
	: TGen::Engine::Script::EventOperation("FRAME", parent)
	, saveContext(true)
	, execute(true)
	, saveReturn(false)
{
	
}

TGen::Engine::Script::FrameOperation::FrameOperation(const std::string & name, TGen::Engine::Script::EventOperation * parent) 
	: TGen::Engine::Script::EventOperation("FRAME:" + name, parent)
	, saveContext(true)
	, execute(true)
	, saveReturn(false)
{
	
}

void TGen::Engine::Script::FrameOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	if (!execute)
		return;
	
	std::cout << ">> FRAME" << std::endl;
	
	if (saveContext) {
		if (saveReturn) {						
			TGen::Engine::TriggerContext savedContext(context);
			TGen::Engine::Script::EventOperation::trigger(context, mode);
			
			int retRegister = context.getReturnRegister();
			
			uint32 retVal = context.getRegister<uint32>(retRegister);
			context = savedContext;
			
			std::cout << "RETURNED " << retVal << std::endl;
			
			context.setRegister(retRegister, retVal);			
		}
		else {
			TGen::Engine::TriggerContext savedContext(context);
			TGen::Engine::Script::EventOperation::trigger(context, mode);
			context = savedContext;
		}
	}
	else {
		TGen::Engine::Script::EventOperation::trigger(context, mode);
	}
	
	std::cout << "<< FRAME" << std::endl;
}

void TGen::Engine::Script::FrameOperation::setSaveContext(bool saveContext) {
	this->saveContext = saveContext;
}

void TGen::Engine::Script::FrameOperation::setExecute(bool execute) {
	this->execute = execute;
}

void TGen::Engine::Script::FrameOperation::setSaveReturn(bool saveReturn) {
	this->saveReturn = saveReturn;
}
