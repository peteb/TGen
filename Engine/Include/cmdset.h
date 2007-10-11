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
		class CommandSet : public TGen::Engine::CommandExecuter {
		public:	
			
			void executeCommand(const TGen::Engine::Command & command, ParameterList & parameters, TGen::Engine::TextOutputer & output);
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CMDSET_H
