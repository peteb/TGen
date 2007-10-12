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
#include "variablesregistry.h"

// parameters: variable, value
// parameters: variable

TGen::Engine::CommandSet::CommandSet(TGen::Engine::VariablesRegistry & variables) 
	: variables(variables)
{	
}

void TGen::Engine::CommandSet::executeCommand(const TGen::Engine::Command & command, ParameterList & parameters, TGen::Engine::TextOutputer & output) {
	if (command.getName() == "set") {	
		if (parameters.size() == 1) {
			std::string varName = parameters[0];
			
			try {
				output.outputText(varName + " = " + variables.getVariable(varName).getValue(), 0);
			}
			catch (TGen::RuntimeException & e) {
				output.outputText(varName + ": " + e.getDescription(), 0);
			}
		}
		else if (parameters.size() == 2) {
			std::string varName = parameters[0];
			std::string varValue = parameters[1];
			//output.outputText("changing var " + varName + " to " + varValue, 0);
			
			variables.addVariable(TGen::Engine::Variable(varName, varValue, "null", TGen::Engine::VariableNonInit), TGen::Engine::AddOverrideValue | TGen::Engine::AddNoThrow);
		}
		else {
			throw TGen::Engine::CommandException(command, "parameter count mismatch, expected: 1 or 2");			
		}
	}
}

