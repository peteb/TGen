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
#include "types.h"

namespace TGen {
	std::string toLower(const std::string & from);
	std::string toUpper(const std::string & from);
	
	std::string getFolder(const std::string & path);
	std::string getFile(const std::string & path);
	std::string getExtension(const std::string & path);
	
	std::string getSize(uint64 bytes);
	
	std::string prependString(const std::string & str, const std::string & prefix);
	std::string prependPath(const std::string & path, const std::string & prefix);
	
} // !TGen

#endif // !_TGEN_STRINGMAN_H
