/*
 *  stringman.cpp
 *  TGen Core
 *
 *  Created by Peter Backman on 7/17/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "stringman.h"

std::string TGen::toLower(const std::string & from) {
	std::string ret;
	std::string::const_iterator it = from.begin();
	
	while (it != from.end()) { 
		ret += ::tolower(*it);	
		++it;
	}
	
	return ret;
}

std::string TGen::toUpper(const std::string & from) {
	std::string ret;
	std::string::const_iterator it = from.begin();
	
	while (it != from.end()) { 
		ret += ::toupper(*it);	
		++it;
	}
	
	return ret;	
}

