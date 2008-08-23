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
	return getSymbolTable()[name];
}

std::string TGen::Engine::getSymbolName(TGen::Engine::Symbol symbol) {
	return getSymbolTable().getSymbolName(symbol);
}

TGen::SymbolTable & TGen::Engine::getSymbolTable() {
	static TGen::SymbolTable symbols;

	return symbols;
}
