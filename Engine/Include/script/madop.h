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
				void trigger(TGen::Engine::TriggerContext & context);		
				
				void setFactor(scalar factor);
				void setTerm(scalar term);
				void setSource(int regId);
				void setDest(int regId);
				
				void setIntTerm(int term);
				void setIntMath(bool intMath);
				
			private:
				scalar factor, term;
				int destId, sourceId, intTerm;
				bool intMath;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MADOP_H

