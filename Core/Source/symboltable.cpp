/*
 *  symboltable.cpp
 *  TGen Core
 *
 *  Created by Peter Backman on 8/23/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "symboltable.h"

TGen::SymbolTable::SymbolTable()
	: id(0)
{
}

int TGen::SymbolTable::operator [](const std::string & name) {
	SymbolMap::iterator iter = symbols.find(name);
	if (iter == symbols.end()) {
		symbols.insert(SymbolMap::value_type(name, id++));
		return id - 1;
	}
	
	return iter->second;
}

std::string TGen::SymbolTable::getSymbolName(int symbol) const {
	for (SymbolMap::const_iterator iter = symbols.begin(); iter != symbols.end(); ++iter) {
		if (iter->second == symbol)
			return iter->first;
	}
	
	return "";
}
