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
#include "matrix3x3.h"

class lua_State;

namespace TGen {
	class Vector3;
	class RuntimeException;

	namespace Engine {
		class File;
		class WorldObject;
		class Filesystem;
		
		namespace Script {
			class ScriptState {
			public:
				ScriptState(TGen::Engine::Filesystem & filesystem);
				ScriptState(lua_State * prevm);
				~ScriptState();
				
				lua_State * getState() const;
				
				void loadScriptFile(TGen::Engine::File * file, const std::string & name);				
				
				void setGlobal(const std::string & name);
				void getGlobal(const std::string & name);
				void setMetatable(int index);
				void newTable();
				void * newUserData(int size);
				void pushUserData(void * data);
				void pushFunction(int (*func) (lua_State *L));
				void pushString(const std::string & val);
				void pushNumber(float number);
				void pushNil();
				void pushBoolean(bool boolean);
				
				void pushVector(const TGen::Vector3 & vec);
				void pushMatrix3(const TGen::Matrix3x3 & mat);
				void pushWorldObject(TGen::Engine::WorldObject * object);
				
				void setField(int index, const std::string & name);
				void getField(int index, const std::string & name);
				
				void pop(int count);
				void call(int nargs, int nresults);
				void remove(int index);
				int getStackTop();
				bool isNil(int index);
				bool isString(int index);
				bool isNumber(int index);
				
				void generateError();
				void generateError(const std::string & desc);
				void generateError(const TGen::RuntimeException & error);
				void getTableValue(const std::string & name);
				void setUserData(const std::string & name, void * data);
				void setFunction(const std::string & name, int (*func)(lua_State * L));
				
				
				void * toUserData(int index);
				float toNumber(int index);
				std::string toString(int index, bool callToString = true);
				TGen::Vector3 toVector(int index);
				TGen::Matrix3x3 toMatrix3(int index);
				
				template<typename T>
				T getSelfPointer() {
					getField(1, "_objectSelf");
					
					T ret = reinterpret_cast<T>(toUserData(-1));
					pop(1);
					
					return ret;
				}
				
			private:
				TGen::Engine::Filesystem * getFilesystem() const;
				
				static const char * LuaChunkReader(lua_State * vm, void * data, size_t * size);
				static int luaImport(lua_State * vm);
				
				bool owner;
				lua_State * vm;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPTSTATE_H

