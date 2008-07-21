/*
 *  event.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_EVENT_H
#define _TGEN_ENGINE_SCRIPT_EVENT_H

#include "component.h"
#include "triggerable.h"
#include "eventoperation.h"
#include "triggercontext.h"

namespace TGen {
	namespace Engine {
		namespace Script {
			class Event : public TGen::Engine::Component, public TGen::Engine::Script::EventOperation {
			public:
				Event(const std::string & name, int symbolId);
				
				void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity);
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				void setMinCallInterval(scalar interval);
				
			private:
				int symbolId;
				double minCallTime, lastCall;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_EVENT_H

