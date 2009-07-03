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
	, entityScript(entityScript)
	, source(source)
{
	registerFunction("playSound", luaPlaySound);
	registerFunction("stopAllChannels", luaStopAllChannels);
}

TGen::Engine::Sound::SourceScript::~SourceScript() {

}

void TGen::Engine::Sound::SourceScript::onChannelFinished() {
	TGenAssert(scriptComponent && scriptEntity);
	
	TGen::Engine::Script::ScriptState & scriptState = getScriptState();
	int startStackTop = scriptState.getStackTop();
	pushComponent(scriptState);
	
	scriptState.getField(-1, "onChannelFinished");
	
	if (!scriptState.isNil(-1)) {
		pushComponent(scriptState);
		
		scriptState.call(1, 0);
	}
	
	scriptState.pop(abs(scriptState.getStackTop() - startStackTop));
}	

int TGen::Engine::Sound::SourceScript::luaStopAllChannels(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);	

	SourceScript * self = scriptState.getSelfPointer<SourceScript *>();
	
	self->source->removeAllChannels();
	
	return 0;
}

int TGen::Engine::Sound::SourceScript::luaPlaySound(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);	
	
	SourceScript * self = scriptState.getSelfPointer<SourceScript *>();
	
	scriptState.getField(2, "_objectSelf");	// get _objectSelf pointer for sound object
	TGen::Engine::Sound::Sound * sound = reinterpret_cast<TGen::Engine::Sound::Sound *>(scriptState.toUserData(-1));
	scriptState.pop(1);
	
	if (sound) {
		try {
			self->source->playSound(sound);
		}
		catch (const TGen::RuntimeException & e) {
			scriptState.generateError(e);
		}
	}
	
	
	return 0;
}