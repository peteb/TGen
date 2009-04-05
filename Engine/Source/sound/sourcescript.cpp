/*
 *  sourcescript.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "sound/sourcescript.h"
#include "sound/sound.h"
#include "sound/soundsource.h"

#include "script/entityscript.h"
#include "script/componentscript.h"
#include "script/scriptstate.h"
#include <tgen_core.h>

TGen::Engine::Sound::SourceScript::SourceScript(const std::string & name, TGen::Engine::Sound::Source * source, TGen::Engine::Script::EntityScript * entityScript) 
	: TGen::Engine::Script::ComponentScript(name, entityScript)
	, name(name)
	, entityScript(entityScript)
	, source(source)
{
	registerFunction("playSound", luaPlaySound);
	
}

TGen::Engine::Sound::SourceScript::~SourceScript() {

}

int TGen::Engine::Sound::SourceScript::luaPlaySound(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);	

	SourceScript * self = scriptState.getSelfPointer<SourceScript *>();
	
	scriptState.getField(2, "_objectSelf");	// get _objectSelf pointer for sound object
	TGen::Engine::Sound::Sound * sound = reinterpret_cast<TGen::Engine::Sound::Sound *>(scriptState.toUserData(-1));
	scriptState.pop(1);
	
	if (sound) {
		self->source->playSound(sound);
	}

	
	return 0;
}
