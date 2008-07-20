/*
 *  moveop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/moveop.h"
#include "resourcecomponent.h"

TGen::Engine::Script::MoveOperation::MoveOperation(TGen::Engine::Script::EventOperation * parent)
	: TGen::Engine::Script::EventOperation(parent)
{
}

void TGen::Engine::Script::MoveOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	int destRegId = destId;
	
	if (derefDest) {
		destRegId = *context.getRegister<int *>(destRegId);
	}
	
	if (sourceResource) {
		*context.getRegister<uint32 *>(destRegId) = reinterpret_cast<uint32>(sourceResource->getData());
	}
	else if (intOp) {
		if (imm) {
			*context.getRegister<int *>(destRegId) = sourceImmInt;
		}
		else {
			if (useSwap) {
				std::swap(*context.getRegister<int *>(destRegId), *context.getRegister<int *>(sourceId));
			}
			else {
				*context.getRegister<int *>(destRegId) = *context.getRegister<int *>(sourceId);
			}
		}
	}
	else {
		if (imm) {
			*context.getRegister<scalar *>(destRegId) = sourceImm;
		}
		else {
			if (useSwap) {
				std::swap(*context.getRegister<scalar *>(destRegId), *context.getRegister<scalar *>(sourceId));
			}
			else {
				*context.getRegister<scalar *>(destRegId) = *context.getRegister<scalar *>(sourceId);
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

void TGen::Engine::Script::MoveOperation::setDerefDest(bool derefDest) {
	this->derefDest = derefDest;
}

void TGen::Engine::Script::MoveOperation::linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
	sourceResource.link(entities, entity);
}

void TGen::Engine::Script::MoveOperation::setResourceName(const std::string & resName) {
	sourceResource = resName;
}


