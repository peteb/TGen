/*
 *  moveop.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MOVEOP_H
#define _TGEN_ENGINE_MOVEOP_H

#include "eventoperation.h"

namespace TGen {
	namespace Engine {
		namespace Script {
			class MoveOperation : public TGen::Engine::Script::EventOperation {
			public:
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				
				void setSource(int regId);
				void setDest(int regId);
				void setSourceImm(scalar sourceImm);
				void setSourceImm(int sourceImm);
				void setSwap(bool useSwap);
				void setDerefDest(bool derefDest);
				
			private:
				int sourceId, destId;
				scalar sourceImm;
				int sourceImmInt;
				bool intOp, useSwap, imm, derefDest;
			};
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MOVEOP_H

