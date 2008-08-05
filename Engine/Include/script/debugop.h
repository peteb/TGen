/*
 *  debugop.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 8/5/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_DEBUGOP_H
#define _TGEN_ENGINE_SCRIPT_DEBUGOP_H

#include "eventoperation.h"

namespace TGen {
	namespace Engine {
		namespace Script {
			class DebugOperation : public TGen::Engine::Script::EventOperation {
			public:
				DebugOperation(TGen::Engine::Script::EventOperation * parent);
				
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				void setDebugLevel(bool debugLevel);
				
			private:
				bool debugLevel;
			};			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_DEBUGOP_H

