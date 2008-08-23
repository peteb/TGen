/*
 *  madop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "madop.h"

TGen::Engine::Script::MadOperation::MadOperation(TGen::Engine::Script::EventOperation * parent)
	: TGen::Engine::Script::EventOperation("MAD", parent)
	, destOffset(-1)
	, sourceOffset(-1)
{
}

void TGen::Engine::Script::MadOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	if (!intMath) {
		scalar fixedFactor = factor;
		scalar fixedTerm = term;

		if (factorRegister != -1)
			fixedFactor = context.getRegister<scalar>(factorRegister);
		
		if (termRegister != -1)
			fixedTerm = context.getRegister<scalar>(termRegister);
		
		if (sourceId == destId) {			
			scalar * dest = context.getRegisterReference<scalar>(sourceId);
			scalar * source = context.getRegisterReference<scalar>(sourceId);
			
			if (destOffset != -1) {
				scalar * array = context.getRegisterPtr<scalar *>(destId);
				dest = &array[destOffset];
			}

			if (sourceOffset != -1) {
				scalar * array = context.getRegisterPtr<scalar *>(sourceId);
				source = &array[sourceOffset];
			}
			
			// TODO: context.getScalarWithOffset, context.getIntWithOffset..
			//std::cout << "MULTIPLY " << std::fixed << *value << " BY " << fixedFactor << " (REG: " << sourceId << ")" << std::endl;

			*dest = *source * fixedFactor + fixedTerm;
		}
		else {
			context.setRegister(destId, context.getRegister<scalar>(sourceId) * fixedFactor + fixedTerm);
		}
	}
	else {
		int fixedFactor = intFactor;
		int fixedTerm = intTerm;
		
		if (factorRegister != -1)
			fixedFactor = context.getRegister<int>(factorRegister);
		
		if (termRegister != -1)
			fixedTerm = context.getRegister<int>(termRegister);
		
		if (sourceId == destId) {
			int * value = context.getRegisterReference<int>(sourceId);		
			*value = *value * fixedFactor + fixedTerm;
		}
		else {
			context.setRegister(destId, context.getRegister<int>(sourceId) * fixedFactor + fixedTerm);
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

void TGen::Engine::Script::MadOperation::setSourceOffset(int offset) {
	this->sourceOffset = offset;
}

void TGen::Engine::Script::MadOperation::setDestOffset(int offset) {
	this->destOffset = offset;
}



