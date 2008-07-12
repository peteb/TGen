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

namespace TGen {
	namespace Engine {
		class Triggerable {
		public:	
			virtual ~Triggerable() {}
			
			virtual void trigger(void ** argv, int argc) abstract;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_TRIGGERABLE_H

