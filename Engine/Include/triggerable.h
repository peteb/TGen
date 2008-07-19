/*
 *  triggerable.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_TRIGGERABLE_H
#define _TGEN_ENGINE_TRIGGERABLE_H

#include <tgen_core.h>
#include "triggercontext.h"

namespace TGen {
	namespace Engine {
		class TriggerContext;
		
		enum TriggerMode {
			TriggerPrecise = 1,	// only the defined receiver gets signal
			TriggerFirst,			// receivers are looped, the first one not to return -1 in r0 breaks the loop
			TriggerAll,				// all receivers are looped
		};
		
		class Triggerable {
		public:	
			Triggerable() : context(NULL) {}
			virtual ~Triggerable() {}
			
			virtual void trigger(TGen::Engine::TriggerContext & context, TriggerMode mode) abstract;
			TGen::Engine::TriggerContext * context;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_TRIGGERABLE_H

