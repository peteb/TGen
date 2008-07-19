/*
 *  madop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "madop.h"

void TGen::Engine::Script::MadOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	if (!intMath) {
		scalar fixedFactor = factor;
		scalar fixedTerm = term;

		if (factorRegister != -1)
			fixedFactor = *context.getRegister<scalar *>(factorRegister);
		if (termRegister != -1)
			fixedTerm = *context.getRegister<scalar *>(termRegister);
		
		if (sourceId == destId) {			
			scalar * value = context.getRegister<scalar *>(sourceId);
			*value = *value * fixedFactor + fixedTerm;
		}
		else {
			context.setRegister(destId, *context.getRegister<scalar *>(sourceId) * fixedFactor + fixedTerm);
		}
	}
	else {
		int fixedFactor = intFactor;
		int fixedTerm = intTerm;
		
		if (factorRegister != -1)
			fixedFactor = *context.getRegister<int *>(factorRegister);
		if (termRegister != -1)
			fixedTerm = *context.getRegister<int *>(termRegister);
		
		if (sourceId == destId) {
			int * value = context.getRegister<int *>(sourceId);
			*value = *value * fixedFactor + fixedTerm;
		}
		else {
			context.setRegister(destId, *context.getRegister<int *>(sourceId) * fixedFactor + fixedTerm);
		}
	}
	
	TGen::Engine::Script::EventOperation::trigger(context, mode);
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

void TGen::Engine::Script::MadOperation::setIntFactor(int factor) {
	this->intFactor = factor;
}

void TGen::Engine::Script::MadOperation::setIntMath(bool intMath) {
	this->intMath = intMath;
}

void TGen::Engine::Script::MadOperation::setTermRegister(int regId) {
	this->termRegister = regId;
}

void TGen::Engine::Script::MadOperation::setFactorRegister(int regId) {
	this->factorRegister = regId;
}

