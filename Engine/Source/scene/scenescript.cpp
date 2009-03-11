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
#include "script/scriptstate.h"
#include "scene/node.h"
#include <cstdlib>
#include <tgen_core.h>
#include <tgen_renderer.h>

#include "lua/lua.hpp"

TGen::Engine::Scene::SceneScript::SceneScript(const std::string & name, TGen::Engine::Scene::Node * sceneNode, TGen::Engine::Script::EntityScript * entityScript) 
	: sceneNode(sceneNode)
	, name(name)
{
	TGenAssert(entityScript);
	scriptComponent = entityScript->createScriptComponent(name, this);
	
	scriptComponent->registerFunction("worldPosition", luaWorldPosition);
	scriptComponent->registerFunction("localPosition", luaLocalPosition);
	scriptComponent->registerFunction("setMaterial", luaSetMaterial);

}

TGen::Engine::Scene::SceneScript::~SceneScript() {
	
}

int TGen::Engine::Scene::SceneScript::luaWorldPosition(lua_State * vm) {
	lua_pushstring(vm, "_objectSelf");
	lua_gettable(vm, -2);
		
	TGen::Engine::Scene::SceneScript * self = reinterpret_cast<TGen::Engine::Scene::SceneScript *>(lua_touserdata(vm, -1));
	
	//lua_pushstring(vm, entity->name.c_str());
	TGen::Engine::Scene::Node * sceneNode = self->sceneNode;
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	
	TGen::Vector3 position = sceneNode->getPosition();
	
	scriptState.pushVector(position);
	
	return 1;
}

int TGen::Engine::Scene::SceneScript::luaLocalPosition(lua_State * vm) {
	lua_pushstring(vm, "_objectSelf");
	lua_gettable(vm, -2);
	
	TGen::Engine::Scene::SceneScript * self = reinterpret_cast<TGen::Engine::Scene::SceneScript *>(lua_touserdata(vm, -1));
	
	//lua_pushstring(vm, entity->name.c_str());
	TGen::Engine::Scene::Node * sceneNode = self->sceneNode;
	TGen::SceneNode * node = sceneNode->getSceneNode();
	
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	
	TGen::Vector3 position;
	
	if (node)
		position = node->getLocalPosition();
	
	scriptState.pushVector(position);
	
	return 1;
}

int TGen::Engine::Scene::SceneScript::luaSetMaterial(lua_State * vm) {
	lua_pushstring(vm, "_objectSelf");
	lua_gettable(vm, -3);
	
	TGen::Engine::Scene::SceneScript * self = reinterpret_cast<TGen::Engine::Scene::SceneScript *>(lua_touserdata(vm, -1));
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	TGen::Material * material = reinterpret_cast<TGen::Material *>(lua_touserdata(vm, -2));
	
	std::cout << "Changing to '" << material->getName() << "'..." << std::endl;
	
	self->sceneNode->getSceneNode()->overrideModelMaterials(material);
	
	return 0;
}
