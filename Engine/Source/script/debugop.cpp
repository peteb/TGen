/*
 *  debugop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/5/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "script/debugop.h"
#include <iostream>

TGen::Engine::Script::DebugOperation::DebugOperation(TGen::Engine::Script::EventOperation * parent) 
	: TGen::Engine::Script::EventOperation("DEBUG", parent)
{
	
}

void TGen::Engine::Script::DebugOperation::trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode) {
	std::cout << "-- DEBUG BREAK --------------------------------------" << std::endl;
	//asm("int $3");
	
	std::cout << "context: " << &context << std::endl;
	std::cout << "   num parameters: " << context.numParameters << std::endl;
	std::cout << "   self: " << std::hex << std::setw(8) << context.selfPointer << std::endl;
	std::cout << "   registers: " << std::endl;
	
	for (int i = 0; i < TGen::Engine::MaxRegisters; ++i) {
		std::cout << "   " << i << ": 0x" << std::hex << std::setw(8) << std::setfill('0') << context.getRegister<int>(i) << " (" << context.getRegister<float>(i) << ") ";
		
		/*
		 TypeNULL = 0,
		 TypeInt = 1,
		 TypeScalar = 2,
		 */
		
		switch (context.registerTypes[i]) {
			case TypeNULL:
				std::cout << "NULL";
				break;
				
			case TypeInt:
				std::cout << "INT";
				break;
				
			case TypeScalar:
				std::cout << "SCALAR";
				break;
				
			default:
				std::cout << "UNK";
		}
		
		std::cout << std::endl;
	}
	
	std::cout << "   aliases:" << std::endl;
	
	TGen::Engine::Script::EventOperation * parent = getParent();
	
	int level = 1;
	
	while (parent) {
		const TGen::Engine::Script::EventOperation::AliasMap aliases = parent->getAliases();
		
		for (TGen::Engine::Script::EventOperation::AliasMap::const_iterator iter = aliases.begin(); iter != aliases.end(); ++iter) {
			for (int i = 0; i < level; ++i)
				std::cout << "   ";
			
			std::cout << iter->first << " = " << iter->second << std::endl;
		}
		
		parent = parent->getParent();
		level++;
	}
	
	std::cout << "trace:" << std::endl;

	TGen::Engine::Script::EventOperation * event = this;
	
	while (event) {
		std::cout << "   (" << event << ") " << event->getName() << std::endl;
		
		if (debugLevel) {
			for (int i = 0; i < event->getNumOperations(); ++i) {
				TGen::Engine::Script::EventOperation * levelOp = event->getOperation(i);
				std::cout << "      (" << levelOp << ") " << levelOp->getName() << std::endl;
			}
		}
		
		event = event->getParent();
	}
	
	exit(2);
}

void TGen::Engine::Script::DebugOperation::setDebugLevel(bool debugLevel) {
	this->debugLevel = debugLevel;
}
