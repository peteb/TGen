/*
 *  error.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/6/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "error.h"
#include <string>
#include <sstream>

TGen::RuntimeException::RuntimeException(const std::string & where, const std::string & description) :
	where(where), description(description)
{
	
}

TGen::RuntimeException::~RuntimeException() throw() {
	
}

const char * TGen::RuntimeException::what() const throw() {
	std::stringstream ss;
	ss << "RuntimeException: " << where << ": " << description;
	return ss.str().c_str();
}


