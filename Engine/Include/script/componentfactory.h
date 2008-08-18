/*
 *  componentfactory.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/18/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_COMPONENTFACTORY_H
#define _TGEN_ENGINE_SCRIPT_COMPONENTFACTORY_H

#include "script/event.h"

namespace TGen {
	namespace Engine {
		class ComponentRecipe;
		
		namespace Script {
			class EventOperation;
			class MoveOperation;
			class IfOperation;
			class LuaCallOperation;
			class Subsystem;
			
			class ComponentFactory {
			public:
				ComponentFactory(TGen::Engine::Script::Subsystem & subsystem);
				
				TGen::Engine::Script::Event * createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				TGen::Engine::ComponentRecipe * createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				
			private:
				void createOperation(TGen::Engine::Script::EventOperation & container, const TGen::PropertyTree & properties);
				void createOperations(TGen::Engine::Script::EventOperation & container, const TGen::PropertyTree & properties);
				
				void createCallOperation(const std::string & head, const TGen::PropertyTree & properties, int attributeStart, TGen::Engine::Script::EventOperation & container);
				TGen::Engine::Script::MoveOperation * createMovOperation(const std::string & type, const std::string & source, const std::string & dest, TGen::Engine::Script::EventOperation & container);
				TGen::Engine::Script::IfOperation * createIfOperation(const std::string & type, int attributeStart, const TGen::PropertyTree & properties, TGen::Engine::Script::EventOperation & container);
				TGen::Engine::Script::LuaCallOperation * createLuaCallOperation(const std::string & type, const TGen::PropertyTree & properties, TGen::Engine::Script::EventOperation & container);
				
				static int getRegisterId(const std::string & desc);
				
				TGen::Engine::Script::EventOperation * lastCreatedOp;
				TGen::Engine::Script::Subsystem & subsystem;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_COMPONENTFACTORY_H

