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

TGen::Engine::Script::IfOperation::IfOperation(TGen::Engine::Script::EventOperation * parent)
	: TGen::Engine::Script::FrameOperation("IF", parent)
	, elseBlock(NULL)
	, intOp(0)
{
	setSaveContext(false);
}

void TGen::Engine::Script::IfOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	if (!loop) {
		bool passed = false;
		
		if (intOp)
			passed = testExpressionInt(context);
		else
			passed = testExpression(context);
		
		if (passed)
			TGen::Engine::Script::FrameOperation::trigger(context, mode);

		if (elseBlock)
			elseBlock->setExecute(!passed);
	}
	else {
		if (intOp) {
			while (testExpressionInt(context))
				TGen::Engine::Script::FrameOperation::trigger(context, mode);
		}
		else {
			while (testExpression(context))
				TGen::Engine::Script::FrameOperation::trigger(context, mode);			
		}
	}
	
}

bool TGen::Engine::Script::IfOperation::testExpression(TGen::Engine::TriggerContext & context) {
	scalar paramValue = context.getRegister<scalar>(regId);
	std::cout << "TEST VALUE: " << paramValue << std::endl;
	
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

// TODO: "use" button, but how should it work? maybe like this: when touching a geom, some control from the player is passed
//          to that geom, or to an event predefined by that geom
//				no! beginTouch, endTouch. 
//		nononono, tab creates a ray in front of the player, just like shooting. but it triggers a "use" event instead of onCollision
//		the ray will be removed the next frame
//		kan ju vara en funktion i controller som sköts av script, onUseKeyPressed, så får man spawna en ray själv
// först: fixa så man kan ange entities för en bana i olika filer, rekursera map/entities. 
// sen finns definitions/entites/    definitions/scripts/   de mappas inte in i vanliga trädet under vanliga /map/blabla/ utan de utförs bara..

bool TGen::Engine::Script::IfOperation::testExpressionInt(TGen::Engine::TriggerContext & context) {
	int paramValue = context.getRegister<int>(regId);
	std::cout << "TEST VALUE " << paramValue << " AGAINST " << value << std::endl;
	
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

void TGen::Engine::Script::IfOperation::setIntOp(bool intOp) {
	this->intOp = intOp;
}
