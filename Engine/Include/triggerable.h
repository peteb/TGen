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
		class Triggerable {
		public:	
			virtual ~Triggerable() {}
			
			virtual void trigger(TGen::Engine::TriggerContext & context) abstract;
			TGen::Engine::TriggerContext & getDefaultContext() {return context; }
			
		private:
			TGen::Engine::TriggerContext context;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_TRIGGERABLE_H

