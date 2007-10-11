/*
 *  cmdset.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "cmdset.h"
#include "log.h"

void TGen::Engine::CommandSet::executeCommand(const TGen::Engine::Command & command, ParameterList & parameters, TGen::Engine::TextOutputer & output) {
	output.outputText("hejj", 0);
}
