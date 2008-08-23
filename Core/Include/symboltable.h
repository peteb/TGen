/*
 *  symboltable.h
 *  TGen Core
 *
 *  Created by Peter Backman on 8/23/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_SYMBOLTABLE_H
#define _TGEN_SYMBOLTABLE_H

#include <map>
#include <string>

namespace TGen {
	class SymbolTable {
	public:
		SymbolTable();
		
		int operator [](const std::string & name);
		std::string getSymbolName(int symbol) const;
		
	private:
		typedef std::map<std::string, int> SymbolMap;
		SymbolMap symbols;
		int id;
	};
} // !TGen

#endif // !_TGEN_SYMBOLTABLE_H
