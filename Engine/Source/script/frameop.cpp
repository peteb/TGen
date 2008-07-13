/*
 *  frameop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/frameop.h"

TGen::Engine::Script::FrameOperation::FrameOperation() 
	: saveContext(true)
	, execute(true)
{
	
}

void TGen::Engine::Script::FrameOperation::trigger(TGen::Engine::TriggerContext & context) {
	if (!execute)
		return;
	
	if (saveContext) {
		TGen::Engine::TriggerContext savedContext(context);
		TGen::Engine::Script::EventOperation::trigger(context);
		context = savedContext;
	}
	else {
		TGen::Engine::Script::EventOperation::trigger(context);
	}
}

void TGen::Engine::Script::FrameOperation::setSaveContext(bool saveContext) {
	this->saveContext = saveContext;
}

void TGen::Engine::Script::FrameOperation::setExecute(bool execute) {
	this->execute = execute;
}
