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

TGen::Engine::Physics::GeomScript::GeomScript(const std::string & name, TGen::Engine::Physics::Geom * geom, TGen::Engine::Script::EntityScript * entityScript) 
	: TGen::Engine::Script::ComponentScript(name, entityScript)
	, name(name)
	, geom(geom)
	, entityScript(entityScript)
{
	TGenAssert(entityScript);
	//scriptComponent = entityScript->createScriptComponent(name, this);

	//registerFunction("owner", luaOwner);
	
	std::cout << name << " GEOMSCRIPT: " << this << std::endl;
	
}

TGen::Engine::Physics::GeomScript::~GeomScript() {
	//delete scriptComponent;
}

void TGen::Engine::Physics::GeomScript::onCollision(scalar force, TGen::Engine::Physics::Geom * with) {
	TGenAssert(scriptComponent && scriptEntity);
	
	TGen::Engine::Script::ScriptState & scriptState = beginComponentScript();
	
	scriptState.getField(-1, "onCollision");
	
	if (!scriptState.isNil(-1)) {
		scriptState.getGlobal("entities");
		scriptState.getField(-1, entityScript->getName());
		scriptState.remove(-2);
		
		scriptState.getField(-1, this->name);
		scriptState.remove(-2);
		
		scriptState.pushNumber(force);
		
		if (with && with->getScriptInterface()) {
			scriptState.getGlobal("entities");
			scriptState.getField(-1, with->getScriptInterface()->entityScript->getName());
			scriptState.remove(-2);
			
			scriptState.getField(-1, with->getScriptInterface()->name);
			scriptState.remove(-2);
		}
		
		scriptState.call(3, 0);
	}
	else {
		scriptState.pop(1);
	}	
	
	endComponentScript();	
}

