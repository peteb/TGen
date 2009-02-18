/*
 *  scenescript.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/17/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#include "scene/scenescript.h"
#include "script/entityscript.h"
#include "script/componentscript.h"
#include "scene/node.h"
#include <cstdlib>
#include <tgen_core.h>

#include "lua/lua.hpp"

TGen::Engine::Scene::SceneScript::SceneScript(const std::string & name, TGen::Engine::Scene::Node * sceneNode, TGen::Engine::Script::EntityScript * entityScript) 
	: sceneNode(sceneNode)
	, name(name)
{
	TGenAssert(entityScript);
	scriptComponent = entityScript->createScriptComponent(name, this);
	
	scriptComponent->registerFunction("getPosition", luaGetPosition);
}

TGen::Engine::Scene::SceneScript::~SceneScript() {
	
}

int TGen::Engine::Scene::SceneScript::luaGetPosition(lua_State * vm) {
	lua_pushstring(vm, "_objectSelf");
	lua_gettable(vm, -2);
	
	// TODO: LuaState-klass som wrappar lua_State och som har pushVector osv, pushVector pushar alla nummer i en table och sätter metatable till vector
	
	TGen::Engine::Scene::SceneScript * self = reinterpret_cast<TGen::Engine::Scene::SceneScript *>(lua_touserdata(vm, -1));
	
	//lua_pushstring(vm, entity->name.c_str());
	TGen::Engine::Scene::Node * sceneNode = self->sceneNode;

	TGen::Vector3 position = sceneNode->getPosition();
	
	lua_createtable(vm, 3, 3);
	
	lua_pushnumber(vm, position.x);
	lua_setfield(vm, -2, "x");

	lua_pushnumber(vm, position.y);
	lua_setfield(vm, -2, "y");

	lua_pushnumber(vm, position.z);
	lua_setfield(vm, -2, "z");
	
	
	return 1;
}


