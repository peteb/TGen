/*
 *  madop.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MADOP_H
#define _TGEN_ENGINE_MADOP_H

#include "eventoperation.h"

namespace TGen {
	namespace Engine {
		namespace Script {
			class MadOperation : public TGen::Engine::Script::EventOperation {
			public:
				void trigger(void ** argv, int argc);		
				
				void setFactor(scalar factor);
				void setTerm(scalar term);
				void setParameter(int parameter);
				
			private:
				scalar factor, term;
				int parameter;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MADOP_H

