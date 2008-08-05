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
#include "symbols.h"

namespace TGen {
	namespace Engine {
		namespace Script {
			class Event : public TGen::Engine::Component, public TGen::Engine::Script::EventOperation {
			public:
				Event(const std::string & name, TGen::Engine::Symbol symbolId);
				
				void link(const TGen::Engine::ComponentLinker & linker);
				void prelink(const TGen::Engine::ComponentLinker & linker);
				
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				void setMinCallInterval(scalar interval);
				TGen::Engine::Symbol getSymbol() const;
				
			private:
				TGen::Engine::Symbol symbolId;
				double minCallTime, lastCall;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_EVENT_H

