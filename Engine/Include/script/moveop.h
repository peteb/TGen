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
#include "componentlink.h"

namespace TGen {
	namespace Engine {
		class ResourceComponent;
		
		namespace Script {
			class MoveOperation : public TGen::Engine::Script::EventOperation {
			public:
				MoveOperation(TGen::Engine::Script::EventOperation * parent);
				
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity);

				void setSource(int regId);
				void setDest(int regId);
				void setSourceImm(scalar sourceImm);
				void setSourceImm(int sourceImm);
				void setSwap(bool useSwap);
				void setDerefDest(bool derefDest);
				void setResourceName(const std::string & resName);
				void setDestOffset(int destOffset);
				void setSourceOffset(int sourceOffset);
				
			private:
				int sourceId, destId;
				scalar sourceImm;
				TGen::Engine::UnaryDelegate<TGen::Engine::ResourceComponent> sourceResource;
				int sourceImmInt, destOffset, sourceOffset;
				bool intOp, useSwap, imm, derefDest;
			};
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MOVEOP_H

