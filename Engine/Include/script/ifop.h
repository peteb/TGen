/*
 *  ifop.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_IFOP_H
#define _TGEN_ENGINE_SCRIPT_IFOP_H

#include "eventoperation.h"

namespace TGen {
	namespace Engine {
		namespace Script {
			class FrameOperation;
			
			enum CompareType {
				CompareEquals = 1,
				CompareNotEquals,
				CompareLessThan,
				CompareLessThanOrEqual,
				CompareGreaterThan,
				CompareGreaterThanOrEqual,
			};
			
			class IfOperation : public TGen::Engine::Script::EventOperation {
			public:
				IfOperation(TGen::Engine::Script::EventOperation * parent);
				
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				
				void setRegister(int regId);
				void setType(CompareType type);
				void setValue(scalar value);
				void setLoop(bool loop);
				void setElseBlock(TGen::Engine::Script::FrameOperation * elseBlock);
				void setIntOp(bool intOp);
				
			private:
				bool testExpression(TGen::Engine::TriggerContext & context);
				bool testExpressionInt(TGen::Engine::TriggerContext & context);
				
				TGen::Engine::Script::FrameOperation * elseBlock;
				
				int regId;
				CompareType type;
				scalar value;
				bool loop, intOp;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_IFOP_H

