/*
 *  cmddumpvars.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "cmddumpvars.h"
#include "variablesregistry.h"
#include "log.h"

TGen::Engine::CommandDumpVars::CommandDumpVars(TGen::Engine::VariablesRegistry & variables) 
	: variables(variables)
{
}


void TGen::Engine::CommandDumpVars::executeCommand(const TGen::Engine::Command & command, ParameterList & parameters, TGen::Engine::TextOutputer & output) {
	if (command.getName() == "dumpvars") {
		std::vector<std::string> vars;
		vars.reserve(100);
		variables.getVariables(vars);
		
		for (int i = 0; i < vars.size(); ++i) {
			TGen::Engine::Variable & variable = variables.getVariable(vars[i]);
			
			std::stringstream ss;
			ss << vars[i] << "\t\t\t" << variable.getValue() << "\t\t\t" << variable.getDefaultValue() << "\t\t\t" << variable.getFlags();
			output.outputText(ss.str(), 0);
		}
	}
}

