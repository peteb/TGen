/*
 *  commandregistry.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_COMMANDREGISTRY_H
#define _TGEN_ENGINE_COMMANDREGISTRY_H

#include "command.h"

namespace TGen {
	namespace Engine {
		class CommandRegistry {
		public:	
			CommandRegistry();
			~CommandRegistry();
			
			TGen::Engine::CommandRegistry & addCommand(TGen::Engine::Command * command, bool doThrow = true);
			TGen::Engine::Command & getCommand(const std::string & name);
			
			TGen::Engine::Command & operator [] (const std::string & name);

		private:
			typedef std::map<std::string, TGen::Engine::Command *> CommandMap;
			CommandMap commands;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_COMMANDREGISTRY_H
