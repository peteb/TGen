/*
 *  madop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "madop.h"

void TGen::Engine::Script::MadOperation::trigger(TGen::Engine::TriggerContext & context) {
	if (!intMath) {
		if (sourceId == destId) {
			scalar * value = context.getRegister<scalar *>(sourceId);
			*value = *value * factor + term;
		}
		else {
			context.setRegister(destId, *context.getRegister<scalar *>(sourceId) * factor + term);
		}
	}
	else {
		if (sourceId == destId) {
			int * value = context.getRegister<int *>(sourceId);
			*value += intTerm;
		}
		else {
			context.setRegister(destId, *context.getRegister<int *>(sourceId) + intTerm);
		}
	}
	
	TGen::Engine::Script::EventOperation::trigger(context);
}

void TGen::Engine::Script::MadOperation::setFactor(scalar factor) {
	this->factor = factor;
}

void TGen::Engine::Script::MadOperation::setTerm(scalar term) {
	this->term = term;
}

void TGen::Engine::Script::MadOperation::setSource(int regId) {
	this->sourceId = regId;
}

void TGen::Engine::Script::MadOperation::setDest(int regId) {
	this->destId = regId;
}

void TGen::Engine::Script::MadOperation::setIntTerm(int term) {
	this->intTerm = term;
}

void TGen::Engine::Script::MadOperation::setIntMath(bool intMath) {
	this->intMath = intMath;
}
