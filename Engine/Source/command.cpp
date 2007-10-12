/*
 *  command.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "command.h"
#include "textoutput.h"

TGen::Engine::Command::Command(const std::string & name, TGen::Engine::CommandExecuter * executer)
	: name(name)
	, executer(executer)
{
		
}

TGen::Engine::Command::~Command() {
	delete executer;
}

void TGen::Engine::Command::execute(TGen::Engine::CommandExecuter::ParameterList & parameters, TGen::Engine::TextOutputer & output) {
	if (executer) {
		executer->executeCommand(*this, parameters, output);
	}
}

std::string TGen::Engine::Command::getName() const {
	return name;
}

TGen::Engine::CommandException::CommandException(const TGen::Engine::Command & command, const std::string & description) 
	: TGen::RuntimeException("cmd_" + command.getName(), description)
	, command(command)
{
}

TGen::Engine::CommandException::~CommandException() throw() {
	
}


const TGen::Engine::Command & TGen::Engine::CommandException::getCommand() const {
	return command;
}
