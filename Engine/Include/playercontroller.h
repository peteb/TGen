/*
 *  playercontroller.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/11/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PLAYERCONTROLLER_H
#define _TGEN_ENGINE_PLAYERCONTROLLER_H

#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		enum Event {
			EventForward = 1,
			EventBackward,
			EventStrafeLeft,
			EventStrafeRight,			
		};
		
		enum EventFlags {
			EventTriggered =	0x1000,
			EventKilled =		0x2000,
			EventRead =			0x4000,
			
		};
		
		class PlayerController {
		public:
			PlayerController();
			
			void beginEvent(int id);
			void endEvent(int id);
			void update(scalar dt);
			
		private:
			uint32 activeEvents[20];
			bool checkEvent(int id);
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PLAYERCONTROLLER_H

