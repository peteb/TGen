/*
 *  stringman.h
 *  TGen Core
 *
 *  Created by Peter Backman on 7/17/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_STRINGMAN_H
#define _TGEN_STRINGMAN_H

#include <string>

namespace TGen {
	std::string toLower(const std::string & from);
	std::string toUpper(const std::string & from);
	
	std::string getFolder(const std::string & path);
	
} // !TGen

#endif // !_TGEN_STRINGMAN_H
