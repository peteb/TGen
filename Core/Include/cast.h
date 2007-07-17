/*
 *  cast.h
 *  TGen Core
 *
 *  Created by Peter Backman on 7/17/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_CAST_H
#define _TGEN_CAST_H

#include "stringman.h"
#include <iostream>

namespace TGen {
	template<typename to, typename from>
	to lexical_cast(const from & _from) {
		std::stringstream ss;
		ss << _from;
		to ret;
		ss >> ret;
		return ret;
	}
	
	/*template<>
	bool lexical_cast<bool, std::string>(const std::string & _from) {
		std::cout << "BOOL CAST" << std::endl;
		std::string lower = TGen::toLower(_from);
		if (lower == "false" || lower == "no" || lower == "0")
			return false;
		
		return true;
	}*/
	
} // !Tgen

#endif // !_TGEN_CAST_H
