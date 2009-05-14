/*
 *  filesystemscript.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/12/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_FILESYSTEMSCRIPT_H
#define _TGEN_ENGINE_FILESYSTEMSCRIPT_H

struct lua_State;

namespace TGen {
	namespace Engine {
		class Filesystem;
		
		namespace Script {
			class Subsystem;
		}
		
		class FilesystemScript {
		public:
			FilesystemScript(TGen::Engine::Filesystem & filesystem, TGen::Engine::Script::Subsystem & scriptSubsystem);
			~FilesystemScript();
			
		private:
			static int luaOpenRead(lua_State * vm);
			static int luaFileClose(lua_State * vm);
			static int luaFileReadAll(lua_State * vm);
			
			TGen::Engine::Filesystem & filesystem;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_FILESYSTEMSCRIPT_H

