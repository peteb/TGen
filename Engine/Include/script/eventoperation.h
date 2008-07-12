/*
 *  eventoperation.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_EVENTOPERATION_H
#define _TGEN_ENGINE_SCRIPT_EVENTOPERATION_H

#include "triggerable.h"

namespace TGen {
	namespace Engine {
		class EntityList;
		class Entity;
		
		namespace Script {
			class EventOperation : public TGen::Engine::Triggerable {
			public:
				virtual ~EventOperation();
				
				void addOperation(EventOperation * operation);
				virtual void trigger(void ** argv, int argc);
				virtual void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity);
				
			private:
				std::vector<EventOperation *> operations;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_EVENTOPERATION_H

