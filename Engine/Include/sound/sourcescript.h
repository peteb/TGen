/*
 *  sourcescript.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SOUND_SOURCESCRIPT_H
#define _TGEN_ENGINE_SOUND_SOURCESCRIPT_H

#include <string>
#include "script/componentscript.h"

struct lua_State;

namespace TGen {
	namespace Engine {
		namespace Script {
			class EntityScript;
		}
		
		namespace Sound {
			class Source;
			
			class SourceScript : public TGen::Engine::Script::ComponentScript {
			public:
				SourceScript(const std::string & name, TGen::Engine::Sound::Source * source, TGen::Engine::Script::EntityScript * entityScript);
				~SourceScript();
				
			private:				
				static int luaPlaySound(lua_State * vm);
				
				TGen::Engine::Script::EntityScript * entityScript;
				TGen::Engine::Sound::Source * source;	
			};
			
		} // !Sound
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SOUND_SOURCESCRIPT_H

