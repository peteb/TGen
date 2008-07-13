/*
 *  moveop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/moveop.h"

void TGen::Engine::Script::MoveOperation::trigger(TGen::Engine::TriggerContext & context) {
	if (intOp) {
		if (imm) {
			*context.getRegister<int *>(destId) = sourceImmInt;
		}
		else {
			if (useSwap) {
				std::swap(*context.getRegister<int *>(destId), *context.getRegister<int *>(sourceId));
			}
			else {
				*context.getRegister<int *>(destId) = *context.getRegister<int *>(sourceId);
			}
		}
	}
	else {
		if (imm) {
			*context.getRegister<scalar *>(destId) = sourceImm;
		}
		else {
			if (useSwap) {
				std::swap(*context.getRegister<scalar *>(destId), *context.getRegister<scalar *>(sourceId));
			}
			else {
				*context.getRegister<scalar *>(destId) = *context.getRegister<scalar *>(sourceId);
			}
		}		
	}
}

void TGen::Engine::Script::MoveOperation::setSource(int regId) {
	this->sourceId = regId;
	this->imm = false;
}

void TGen::Engine::Script::MoveOperation::setDest(int regId) {
	this->destId = regId;
}

void TGen::Engine::Script::MoveOperation::setSourceImm(scalar sourceImm) {
	this->sourceImm = sourceImm;
	this->intOp = false;
	this->imm = true;
}

void TGen::Engine::Script::MoveOperation::setSourceImm(int sourceImm) {
	this->sourceImmInt = sourceImm;
	this->intOp = true;
	this->imm = true;
}

void TGen::Engine::Script::MoveOperation::setSwap(bool useSwap) {
	this->useSwap = useSwap;
}

