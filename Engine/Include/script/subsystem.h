/*
 *  subsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_SUBSYSTEM_H
#define _TGEN_ENGINE_SCRIPT_SUBSYSTEM_H

#include "../subsystem.h"
#include "script/event.h"

namespace TGen {
	namespace Engine {
		class StandardLogs;
		
		namespace Script {	
			class EventOperation;
			class MoveOperation;
			class IfOperation;
			
			class Subsystem : public TGen::Engine::Subsystem {
			public:
				Subsystem(TGen::Engine::StandardLogs & logs);
				~Subsystem();
				
				TGen::Engine::Script::Event * createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				TGen::Engine::ComponentRecipe * createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
								
			private: 
				void createOperation(TGen::Engine::Script::EventOperation & container, const TGen::PropertyTree & properties);
				void createOperations(TGen::Engine::Script::EventOperation & container, const TGen::PropertyTree & properties);
				TGen::Engine::Script::MoveOperation * createMovOperation(const std::string & type, const std::string & source, const std::string & dest, TGen::Engine::Script::EventOperation & container);
				void createCallOperation(const std::string & head, const TGen::PropertyTree & properties, int attributeStart, TGen::Engine::Script::EventOperation & container);
				TGen::Engine::Script::IfOperation * createIfOperation(const std::string & type, int attributeStart, const TGen::PropertyTree & properties, TGen::Engine::Script::EventOperation & container);
				
				int getRegisterId(const std::string & desc);
				
				TGen::Engine::Script::EventOperation * lastCreatedOp;
				TGen::Engine::StandardLogs & logs;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_SUBSYSTEM_H

