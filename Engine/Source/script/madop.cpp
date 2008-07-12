/*
 *  madop.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "madop.h"

void TGen::Engine::Script::MadOperation::trigger(void ** argv, int argc) {
	if (parameter < 0 || parameter >= argc)
		return;
	
	scalar * value = reinterpret_cast<scalar *>(argv[parameter]);
	*value = *value * factor + term;
	
	TGen::Engine::Script::EventOperation::trigger(argv, argc);
}

void TGen::Engine::Script::MadOperation::setFactor(scalar factor) {
	this->factor = factor;
}

void TGen::Engine::Script::MadOperation::setTerm(scalar term) {
	this->term = term;
}

void TGen::Engine::Script::MadOperation::setParameter(int parameter) {
	this->parameter = parameter;
}

