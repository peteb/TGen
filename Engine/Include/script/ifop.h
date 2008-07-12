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
			enum CompareType {
				CompareEquals = 1,
				CompareLessThan,
				CompareLessThanOrEqual,
				CompareGreaterThan,
				CompareGreaterThanOrEqual,
			};
			
			class IfOperation : public TGen::Engine::Script::EventOperation {
			public:
				IfOperation();
				
				void trigger(void ** argv, int argc);
				
				void setParam(int parameter);
				void setType(CompareType type);
				void setValue(scalar value);
				
			private:
				int parameter;
				CompareType type;
				scalar value;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_IFOP_H

