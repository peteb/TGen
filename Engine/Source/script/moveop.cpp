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
	: TGen::Engine::Script::EventOperation("MOV", parent)
	, destOffset(-1)
	, sourceOffset(-1)
	, destId(-1)
	, sourceId(-1)
{
}

void TGen::Engine::Script::MoveOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	int destRegId = destId;
	std::cerr << "EXEC MOVE" << std::endl;
	
	if (derefDest) {
		destRegId = *context.getRegister<int *>(destRegId);
		std::cerr << "DEREF DEST TO: " << std::dec << destRegId << " FROM: " << destId << std::endl;
		std::cerr << "CONTEXT: " << &context << std::endl;
	}
	
	if (destId == 1) {
		std::cout << "WARNING; COPYING TO r1 ";
	}
	
	if (sourceResource) {
		std::cerr << "SOURCE RESOURCE" << std::endl;
		*context.getRegister<uint32 *>(destRegId) = reinterpret_cast<uint32>(sourceResource->getData());
	}
	else if (intOp) {
		std::cout << "INTOP ";
		
		if (imm) {
			std::cout << "IMM " << sourceImmInt;
			
			*context.getRegister<int *>(destRegId) = sourceImmInt;
		}
		else {
			std::cout << "FROM " << sourceId ;
			
			if (useSwap) {
				std::swap(*context.getRegister<int *>(destRegId), *context.getRegister<int *>(sourceId));
			}
			else {
				*context.getRegister<int *>(destRegId) = *context.getRegister<int *>(sourceId);
			}
		}
	}
	else {
		scalar * dest = context.getRegister<scalar *>(destRegId);
		scalar * source = context.getRegister<scalar *>(sourceId);
		
		if (destOffset != -1) {
			scalar * array = *context.getRegister<scalar **>(destRegId);
			dest = &array[destOffset];
		}
		
		if (sourceOffset != -1) {
			scalar * array = *context.getRegister<scalar **>(sourceId);
			source = &array[sourceOffset];
		}
		
		if (imm) {
			*dest = sourceImm;
		}
		else {			
			if (useSwap) {
				std::swap(*dest, *source);
			}
			else {
				*dest = *source;
			}
		}		
	}
	
	std::cout << std::endl;

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

void TGen::Engine::Script::MoveOperation::link(const TGen::Engine::ComponentLinker & linker) {
	sourceResource.link(linker);		// TODO: skicka med EntityList i ComponentRecipe::linkGlobally, och sourceResource ska vara relativt addreserad! inte string!
}

void TGen::Engine::Script::MoveOperation::prelink(const TGen::Engine::ComponentLinker & linker) {
	sourceResource.prelink(linker);
}

void TGen::Engine::Script::MoveOperation::setResourceName(const std::string & resName) {
	sourceResource.set(resName);
}

void TGen::Engine::Script::MoveOperation::setDestOffset(int destOffset) {
	this->destOffset = destOffset;
}

void TGen::Engine::Script::MoveOperation::setSourceOffset(int sourceOffset) {
	this->sourceOffset = sourceOffset;
}


