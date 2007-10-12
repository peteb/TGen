/*
 *  command.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_COMMAND_H
#define _TGEN_ENGINE_COMMAND_H

#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		class TextOutputer;
		class Command;
		
		class CommandException : public TGen::RuntimeException {
		public:
			CommandException(const TGen::Engine::Command & command, const std::string & description);
			~CommandException() throw();
			
			const TGen::Engine::Command & getCommand() const;
			
		private:
			const TGen::Engine::Command & command;
		};
		
		class CommandExecuter {
		public:
			virtual ~CommandExecuter() {}
			
			typedef std::vector<std::string> ParameterList;
			
			virtual void executeCommand(const TGen::Engine::Command & command, ParameterList & parameters, TGen::Engine::TextOutputer & output) abstract;
		};
		
		class Command {
		public:	
			Command(const std::string & name, CommandExecuter * executer);
			~Command();
			
			void execute(TGen::Engine::CommandExecuter::ParameterList & parameters, TGen::Engine::TextOutputer & output);
			std::string getName() const;
			
		private:
			std::string name;
			CommandExecuter * executer;
		};
		
	} // !Engine
} // !TGen



#endif // !_TGEN_ENGINE_COMMAND_H
