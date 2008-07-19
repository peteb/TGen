/*
 *  ifop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/ifop.h"
#include "script/frameop.h"

TGen::Engine::Script::IfOperation::IfOperation()
	: elseBlock(NULL)
{
	
}

void TGen::Engine::Script::IfOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	if (!loop) {
		bool passed = testExpression(context);
		
		if (passed)
			TGen::Engine::Script::EventOperation::trigger(context, mode);

		if (elseBlock)
			elseBlock->setExecute(!passed);
	}
	else {
		while (testExpression(context))
			TGen::Engine::Script::EventOperation::trigger(context, mode);
	}
	
}

bool TGen::Engine::Script::IfOperation::testExpression(TGen::Engine::TriggerContext & context) {
	scalar paramValue = *context.getRegister<scalar *>(regId);
	
	bool passed = false;
	
	switch (type) {
		case CompareEquals:
			passed = (paramValue == value);
			break;
			
		case CompareNotEquals:
			passed = (paramValue != value);
			break;
			
		case CompareLessThan:
			passed = (paramValue < value);
			break;
			
		case CompareLessThanOrEqual:
			passed = (paramValue <= value);
			break;
			
		case CompareGreaterThan:
			passed = (paramValue > value);
			break;
			
		case CompareGreaterThanOrEqual:
			passed = (paramValue >= value);
			break;
	}
	
	return passed;
}

void TGen::Engine::Script::IfOperation::setRegister(int regId) {
	this->regId = regId;
}

void TGen::Engine::Script::IfOperation::setType(TGen::Engine::Script::CompareType type) {
	this->type = type;
}

void TGen::Engine::Script::IfOperation::setValue(scalar value) {
	this->value = value;
}

void TGen::Engine::Script::IfOperation::setLoop(bool loop) {
	this->loop = loop;
}

void TGen::Engine::Script::IfOperation::setElseBlock(TGen::Engine::Script::FrameOperation * elseBlock) {
	this->elseBlock = elseBlock;
}
