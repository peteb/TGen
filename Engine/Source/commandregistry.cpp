/*
 *  commandregistry.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "commandregistry.h"

TGen::Engine::CommandRegistry::CommandRegistry() {
	
}

TGen::Engine::CommandRegistry::~CommandRegistry() {
	for (CommandMap::iterator iter = commands.begin(); iter != commands.end(); ++iter) {
		delete iter->second;
	}
}


TGen::Engine::CommandRegistry & TGen::Engine::CommandRegistry::addCommand(TGen::Engine::Command * command, bool doThrow) {
	CommandMap::const_iterator iter = commands.find(command->getName());
	
	if (iter != commands.end()) {
		if (doThrow)
			throw TGen::RuntimeException("CommandRegistry::addCommand", "command '" + command->getName() + "' already exists");
		else
			return *this;		
	}
	
	commands.insert(CommandMap::value_type(command->getName(), command));
	
	return *this;
}

TGen::Engine::Command & TGen::Engine::CommandRegistry::getCommand(const std::string & name) {
	CommandMap::iterator iter = commands.find(name);
	if (iter == commands.end())
		throw TGen::RuntimeException("CommandRegistry::getCommand", "command '" + name + "' does not exist");
	
	return *iter->second;
}

TGen::Engine::Command & TGen::Engine::CommandRegistry::operator [] (const std::string & name) {
	return getCommand(name);
}
