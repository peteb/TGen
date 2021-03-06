/*
 *  cmddumpvars.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_CMDDUMPVARS_H
#define _TGEN_ENGINE_CMDDUMPVARS_H

#include "command.h"

namespace TGen {
	namespace Engine {
		class VariableRegister;
		
		class CommandDumpVars : public TGen::Engine::CommandExecuter {
		public:	
			CommandDumpVars(TGen::Engine::VariableRegister & variables);
			
			void executeCommand(const TGen::Engine::Command & command, ParameterList & parameters, TGen::Engine::TextOutputer & output);
			
		private:
			TGen::Engine::VariableRegister & variables;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CMDDUMPVARS_H
