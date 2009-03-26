/*
 *  envscript.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_ENVSCRIPT_H
#define _TGEN_ENGINE_SCRIPT_ENVSCRIPT_H

struct lua_State;

namespace TGen {
	namespace Engine {
		namespace Script {
			class ScriptState;
			
			class EnvironmentScript {
			public:
				EnvironmentScript(TGen::Engine::Script::ScriptState & scriptState);
				~EnvironmentScript();
				
			private:
				static int luaShowMessage(lua_State * vm);
				static int luaQuit(lua_State * vm);

			};
		}
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_ENVSCRIPT_H

