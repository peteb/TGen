/*
 *  frameop.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCRIPT_FRAMEOP_H
#define _TGEN_ENGINE_SCRIPT_FRAMEOP_H

#include "eventoperation.h"

namespace TGen {
	namespace Engine {
		namespace Script {
			class FrameOperation : public TGen::Engine::Script::EventOperation {
			public:
				FrameOperation(TGen::Engine::Script::EventOperation * parent);
				
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				
				void setSaveContext(bool saveContext);
				void setExecute(bool execute);
				void setSaveReturn(bool saveReturn);
				
			private:
				bool saveContext, execute, saveReturn;
			};
			
		} // !Script
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCRIPT_FRAMEOP_H

