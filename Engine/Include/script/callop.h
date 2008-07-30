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
#include "componentlink.h"

namespace TGen {
	namespace Engine {
		namespace Script {
			class CallOperation : public TGen::Engine::Script::EventOperation {
			public:
				CallOperation(TGen::Engine::Script::EventOperation * parent);
				
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				void link(const TGen::Engine::ComponentLinker & linker);
				void linkRecipe(const TGen::Engine::EntityRecipe & recipe);
				
				void setEvent(const std::string & eventName);
				void setEvent(int regId);
				void setEntity(const std::string & entityName);
				void setShareContext(bool shareContext);
				void setOffset(int offset);
				void setTriggerMode(TGen::Engine::TriggerMode triggerMode);
				void setNumParameters(int numParams);
				
			private:
				TGen::Engine::TriggerMode triggerMode;
				std::string eventName, entityName;				
				bool shareContext;
				TGen::Engine::Triggerable * event;
				int registerId, offset, numParameters;
				int componentId;
				
				TGen::Engine::UnaryDelegate<TGen::Engine::Component> comp;
				
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_CALLOP_H

