/*
 *  scriptstate.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/18/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPTSTATE_H
#define _TGEN_ENGINE_SCRIPTSTATE_H

#include <string>

class lua_State;

namespace TGen {
	class Vector3;

	namespace Engine {
		class File;
		
		namespace Script {
			class ScriptState {
			public:
				ScriptState();
				ScriptState(lua_State * prevm);
				~ScriptState();
				
				lua_State * getState() const;
				
				void loadScriptFile(TGen::Engine::File * file, const std::string & name);				
				
				void getGlobal(const std::string & name);
				void newTable();
				void pushUserData(void * data);
				void pushFunction(int (*func) (lua_State *L));
				void pushString(const std::string & val);
				
				void pushVector(const TGen::Vector3 & vec);
				
				
				void setField(int index, const std::string & name);
				void getField(int index, const std::string & name);
				
				void pop(int count);
				void call(int nargs, int nresults);
				
				void getTableValue(const std::string & name);
				void setUserData(const std::string & name, void * data);
				void setFunction(const std::string & name, int (*func)(lua_State * L));
				
				
				void * toUserData(int index);
				
			private:
				static const char * LuaChunkReader(lua_State * vm, void * data, size_t * size);

				bool owner;
				lua_State * vm;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPTSTATE_H

