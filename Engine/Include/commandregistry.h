/*
 *  commandregister.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_COMMANDREGISTER_H
#define _TGEN_ENGINE_COMMANDREGISTER_H

#include "command.h"
#include "tokenizer.h"

namespace TGen {
	namespace Engine {
		class Filesystem;
		
		enum CommandTokens {
			CommandTokenEOL = 20,
		};	
		
		class CommandRegister {
		public:	
			CommandRegister();
			~CommandRegister();
			
			TGen::Engine::CommandRegister & addCommand(TGen::Engine::Command * command, bool doThrow = true);
			TGen::Engine::Command & getCommand(const std::string & name);
			
			TGen::Engine::Command & operator [] (const std::string & name);
			void executeFile(const std::string & filename, TGen::Engine::Filesystem & fs, TGen::Engine::TextOutputer & output);
			void execute(const std::string & text, TGen::Engine::TextOutputer & output);
			
		private:
			typedef std::map<std::string, TGen::Engine::Command *> CommandMap;
			CommandMap commands;
		};
		
		class CommandTokenizer : public TGen::Tokenizer {
		public:
			CommandTokenizer();
			
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMMANDREGISTER_H
