/*
 *  scenescript.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/17/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCENESCRIPT_H
#define _TGEN_ENGINE_SCENESCRIPT_H

#include <string>

class lua_State;

namespace TGen {
	namespace Engine {
		namespace Script {
			class EntityScript;
			class ComponentScript;
		}
		
		namespace Scene {
			class Node;
			
			class SceneScript {
			public:
				SceneScript(const std::string & name, TGen::Engine::Scene::Node * sceneNode, TGen::Engine::Script::EntityScript * entityScript);
				~SceneScript();
				
			private:
				static int luaWorldPosition(lua_State * vm);
				static int luaLocalPosition(lua_State * vm);
				static int luaSetMaterial(lua_State * vm);
				static int luaSetWorldPosition(lua_State * vm);
				static int luaSetLocalPosition(lua_State * vm);
				static int luaSetLocalOrientation(lua_State * vm);
				
				TGen::Engine::Script::ComponentScript * scriptComponent;
				TGen::Engine::Scene::Node * sceneNode;
				std::string name;
			};
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENESCRIPT_H
