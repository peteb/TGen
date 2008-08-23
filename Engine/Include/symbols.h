/*
 *  symbols.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SYMBOLS_H
#define _TGEN_ENGINE_SYMBOLS_H

#include <string>

namespace TGen {
	class SymbolTable;
	
	namespace Engine {
		typedef int Symbol;
		
		Symbol getUniqueSymbol(const std::string & name);
		std::string getSymbolName(Symbol symbol);

		TGen::SymbolTable & getSymbolTable();
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SYMBOLS_H

