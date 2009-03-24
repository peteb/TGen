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
	scriptComponent->registerFunction("setWorldPosition", luaSetWorldPosition);
	scriptComponent->registerFunction("setLocalPosition", luaSetLocalPosition);
	scriptComponent->registerFunction("setLocalOrientation", luaSetLocalOrientation);

}

TGen::Engine::Scene::SceneScript::~SceneScript() {
	delete scriptComponent;
}

int TGen::Engine::Scene::SceneScript::luaWorldPosition(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	
	SceneScript * self = scriptState.getSelfPointer<SceneScript *>();
	
	TGen::Engine::Scene::Node * sceneNode = self->sceneNode;
	
	
	TGen::Vector3 position = sceneNode->getPosition();
	
	scriptState.pushVector(position);
	
	return 1;
}

int TGen::Engine::Scene::SceneScript::luaLocalPosition(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);

	SceneScript * self = scriptState.getSelfPointer<SceneScript *>();
	
	TGen::Engine::Scene::Node * sceneNode = self->sceneNode;
	TGen::SceneNode * node = sceneNode->getSceneNode();
	
	
	
	TGen::Vector3 position;
	
	if (node)
		position = node->getLocalPosition();
	
	scriptState.pushVector(position);
	
	return 1;
}

int TGen::Engine::Scene::SceneScript::luaSetMaterial(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);

	SceneScript * self = scriptState.getSelfPointer<SceneScript *>();
	
	scriptState.getField(2, "_objectSelf");	// get _objectSelf pointer for material object
	TGen::Material * material = reinterpret_cast<TGen::Material *>(lua_touserdata(vm, -1));
	scriptState.pop(2);
	
	//std::cout << "Changing to '" << material->getName() << "'..." << std::endl;
	
	if (self && self->sceneNode && self->sceneNode->getSceneNode())
		self->sceneNode->getSceneNode()->overrideModelMaterials(material);
	
	return 0;
}

int TGen::Engine::Scene::SceneScript::luaSetWorldPosition(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	SceneScript * self = scriptState.getSelfPointer<SceneScript *>();
	
	TGen::Vector3 vec = scriptState.toVector(2);
	self->sceneNode->setPosition(vec);
	
	return 0;
}

int TGen::Engine::Scene::SceneScript::luaSetLocalPosition(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	SceneScript * self = scriptState.getSelfPointer<SceneScript *>();
	
	TGen::Vector3 vec = scriptState.toVector(2);
	self->sceneNode->getSceneNode()->setPosition(vec);
	
	return 0;
}

int TGen::Engine::Scene::SceneScript::luaSetLocalOrientation(lua_State * vm) {
	TGen::Engine::Script::ScriptState scriptState(vm);
	SceneScript * self = scriptState.getSelfPointer<SceneScript *>();
	
	TGen::Rotation orientation = scriptState.toMatrix3(2);
	self->sceneNode->getSceneNode()->setOrientation(orientation);
	
	return 0;
}

