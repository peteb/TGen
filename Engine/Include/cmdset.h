/*
 *  cmdset.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_CMDSET_H
#define _TGEN_ENGINE_CMDSET_H

#include "command.h"

namespace TGen {
	namespace Engine {
		class VariablesRegistry;
		
		class CommandSet : public TGen::Engine::CommandExecuter {
		public:	
			CommandSet(TGen::Engine::VariablesRegistry & variables);
			
			void executeCommand(const TGen::Engine::Command & command, ParameterList & parameters, TGen::Engine::TextOutputer & output);
			
		private:
			TGen::Engine::VariablesRegistry & variables;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CMDSET_H