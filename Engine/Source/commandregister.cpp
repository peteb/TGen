/*
 *  commandregister.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "commandregister.h"
#include "log.h"
#include "filesystem.h"
#include "file.h"

TGen::Engine::CommandRegister::CommandRegister() {
	
}

TGen::Engine::CommandRegister::~CommandRegister() {
	for (CommandMap::iterator iter = commands.begin(); iter != commands.end(); ++iter) {
		delete iter->second;
	}
}


TGen::Engine::CommandRegister & TGen::Engine::CommandRegister::addCommand(TGen::Engine::Command * command, bool doThrow) {
	CommandMap::const_iterator iter = commands.find(command->getName());
	
	if (iter != commands.end()) {
		if (doThrow)
			throw TGen::RuntimeException("CommandRegister::addCommand", "command '" + command->getName() + "' already exists");
		else
			return *this;		
	}
	
	commands.insert(CommandMap::value_type(command->getName(), command));
	
	return *this;
}

TGen::Engine::Command & TGen::Engine::CommandRegister::getCommand(const std::string & name) {
	CommandMap::iterator iter = commands.find(name);
	if (iter == commands.end())
		throw TGen::RuntimeException("CommandRegister::getCommand", "command '" + name + "' does not exist");
	
	return *iter->second;
}

TGen::Engine::Command & TGen::Engine::CommandRegistry::operator [] (const std::string & name) {
	return getCommand(name);
}

void TGen::Engine::CommandRegister::executeFile(const std::string & filename, TGen::Engine::Filesystem & fs, TGen::Engine::TextOutputer & output) {
	output.outputText("executing " + filename + "...", 0);
	
	TGen::Engine::File * file = fs.openRead(filename);
	if (!file)
		throw TGen::RuntimeException("CommandRegister::executeFile", "couldn't open file " + filename);
	
	std::string text = file->readAll();
	delete file;
	
	execute(text, output);
}

void TGen::Engine::CommandRegister::execute(const std::string & text, TGen::Engine::TextOutputer & output) {
	//output.outputText("EXEC: " + text, 0);
	
	TGen::Engine::CommandTokenizer tokenizer;
	TGen::TokenStream tokens;
	TGen::TokenStream::TokenList::iterator currentToken, endIter;		

	tokenizer.tokenizeString(text, tokens, false);
	currentToken = tokens.getFirstToken();
	endIter = tokens.getEndToken();
	
	std::string commandName;
	std::vector<std::string> parameters;
	parameters.reserve(4);
	
	while (currentToken != endIter) {
		if (currentToken->first == TGen::Engine::CommandTokenEOL) {
			if (!commandName.empty()) {
				getCommand(commandName).execute(parameters, output);
				commandName = "";
				parameters.clear();
			}
		}
		else {
			if (commandName.empty())
				commandName = currentToken->second;
			else
				parameters.push_back(currentToken->second);
		}
		
		currentToken++;
	}
}

TGen::Engine::CommandTokenizer::CommandTokenizer() {
	autoAddTilNextIgnore = true;
	
	ignores.push_back(" ");
	ignores.push_back("\t");
	ignores.push_back("\r");
	
	quotes.push_back(StringPair("\"", "\""));
	comments.push_back(StringPair("//", "\n"));
	comments.push_back(StringPair("/*", "*/"));
	
	tokens["\n"] = TGen::Engine::CommandTokenEOL;
	tokens[";"] = TGen::Engine::CommandTokenEOL;
}

