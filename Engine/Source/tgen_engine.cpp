/*
 *  tgen_engine.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/27/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "tgen_engine.h"
#include "variablesregistry.h"

bool TGen::isEngineDebug() {
	#ifdef _DEBUG
	return true;
	#else
	return false;
	#endif
}

std::string TGen::Engine::getVersionString() {
	std::stringstream ss;
	ss << VersionMajor << "." << VersionMinor << "." << VersionRevision;
	return ss.str();
}
