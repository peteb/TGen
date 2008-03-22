/*
 *  state.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_STATE_H
#define _TGEN_ENGINE_STATE_H

#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		class State {
		public:
			State();
			virtual ~State();
			
			virtual void tick() abstract;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_STATE_H
