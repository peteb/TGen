/*
 *  symbols.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "symbols.h"
#include "symboltable.h"


TGen::Engine::Symbol TGen::Engine::getUniqueSymbol(const std::string & name) {
	static TGen::SymbolTable symbols;

	return symbols[name];
}

