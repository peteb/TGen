/*
 *  ifop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/ifop.h"

TGen::Engine::Script::IfOperation::IfOperation() {
	
}

void TGen::Engine::Script::IfOperation::trigger(void ** argv, int argc) {
	scalar paramValue = *reinterpret_cast<scalar *>(argv[parameter]);
	
	bool passed = false;

	switch (type) {
		case CompareEquals:
			passed = (paramValue == value);
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
		
	uint32 savedStack[10];
	for (int i = 0; i < argc; ++i)
		savedStack[i] = *reinterpret_cast<uint32 *>(argv[i]);
	
	if (passed)
		TGen::Engine::Script::EventOperation::trigger(argv, argc);
	
	for (int i = 0; i < argc; ++i)
		*reinterpret_cast<uint32 *>(argv[i]) = savedStack[i];
}


void TGen::Engine::Script::IfOperation::setParam(int parameter) {
	this->parameter = parameter;
}

void TGen::Engine::Script::IfOperation::setType(TGen::Engine::Script::CompareType type) {
	this->type = type;
}

void TGen::Engine::Script::IfOperation::setValue(scalar value) {
	this->value = value;
}

