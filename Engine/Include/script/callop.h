/*
 *  callop.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_CALLOP_H
#define _TGEN_ENGINE_SCRIPT_CALLOP_H

#include "eventoperation.h"

namespace TGen {
	namespace Engine {
		namespace Script {
			class CallOperation : public TGen::Engine::Script::EventOperation {
			public:
				CallOperation(TGen::Engine::Script::EventOperation * parent);
				
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity);
				
				void setEvent(const std::string & eventName);
				void setEvent(int regId);
				void setShareContext(bool shareContext);
				void setOffset(int offset);
				void setTriggerMode(TGen::Engine::TriggerMode triggerMode);
				
			private:
				TGen::Engine::TriggerMode triggerMode;
				std::string eventName;				
				bool shareContext;
				TGen::Engine::Triggerable * event;
				int registerId, offset;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_CALLOP_H

