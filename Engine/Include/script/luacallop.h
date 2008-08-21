/*
 *  luacallop.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/18/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_LUACALLOP_H
#define _TGEN_ENGINE_SCRIPT_LUACALLOP_H

#include "eventoperation.h"

struct lua_State;

namespace TGen {
	namespace Engine {
		namespace Script {
			class Subsystem;
			
			class LuaCallOperation : public TGen::Engine::Script::EventOperation {
			public:
				LuaCallOperation(TGen::Engine::Script::EventOperation * parent, TGen::Engine::Script::Subsystem & subsystem);
				
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);	
				void setFunctionName(const std::string & functionName);
				void addReturnToRegister(int registerId);
				void addParameterRegister(int registerId);
				
			private:
				std::string functionName;
				lua_State * luaContext;
				TGen::Engine::Script::Subsystem & subsystem;
				int returnToRegister;
				std::vector<int> parameters;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_LUACALLOP_H

