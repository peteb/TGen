/*
 *  geomscript.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/24/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "physics/geomscript.h"
#include "script/componentscript.h"
#include "script/scriptstate.h"
#include "physics/geom.h"
#include "physics/bodyscript.h"

TGen::Engine::Physics::GeomScript::GeomScript(const std::string & name, TGen::Engine::Physics::Geom * geom, TGen::Engine::Script::EntityScript * entityScript) 
	: TGen::Engine::Script::ComponentScript(name, entityScript)
	, geom(geom)
	, entityScript(entityScript)
{
	TGenAssert(entityScript);
	
	registerFunction("link", luaLink);
}

TGen::Engine::Physics::GeomScript::~GeomScript() {

}

void TGen::Engine::Physics::GeomScript::onCollision(scalar force, TGen::Engine::Physics::Geom * with) {
	TGenAssert(scriptEntity);
	
	TGen::Engine::Script::ScriptState & scriptState = getScriptState();
	int startStackTop = scriptState.getStackTop();
	pushComponent(scriptState);
	
	scriptState.getField(-1, "onCollision");
	
	if (!scriptState.isNil(-1)) {
		pushComponent(scriptState);
		scriptState.pushNumber(force);
		
		if (with && with->getScriptInterface()) {
			scriptState.getGlobal("entities");
			scriptState.getField(-1, with->getScriptInterface()->entityScript->getName());
			scriptState.remove(-2);
			
			scriptState.getField(-1, with->getScriptInterface()->getName());
			scriptState.remove(-2);
		}
		
		scriptState.call(3, 0);
	}
	
	scriptState.pop(abs(scriptState.getStackTop() - startStackTop));
}

void TGen::Engine::Physics::GeomScript::onFirstCollision(TGen::Engine::Physics::Geom * with) {
	TGenAssert(scriptEntity);
	
	TGen::Engine::Script::ScriptState & scriptState = getScriptState();
	int startStackTop = scriptState.getStackTop();
	pushComponent(scriptState);
	
	scriptState.getField(-1, "onFirstCollision");
	
	if (!scriptState.isNil(-1)) {
		pushComponent(scriptState);
		
		if (with && with->getScriptInterface()) {
			scriptState.getGlobal("entities");
			scriptState.getField(-1, with->getScriptInterface()->entityScript->getName());
			scriptState.remove(-2);
			
			scriptState.getField(-1, with->getScriptInterface()->getName());
			scriptState.remove(-2);
		}
		
		scriptState.call(2, 0);
	}
	
	scriptState.pop(abs(scriptState.getStackTop() - startStackTop));	
}

int TGen::Engine::Physics::GeomScript::luaLink(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	GeomScript * self = scriptState.getSelfPointer<GeomScript *>();
	
	if (!self->geom->getBody()) {
		if (!self->geom->getWorldLink()) {
			scriptState.pushNil();
		}
		else {
			scriptState.pushWorldObject(self->geom->getWorldLink());
		}
	}
	else {
		self->geom->getBody()->getScriptInterface()->pushComponent(scriptState);
	}
	
	// TODO: setWorldPosition, setWorldOrientation
	
	return 1;
}
