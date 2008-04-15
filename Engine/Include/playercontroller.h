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
#include "component.h"

namespace TGen {
	class Camera;
	class SceneNode;
	
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
		
		class PlayerController : public TGen::Engine::Component {
		public:
			PlayerController(const std::string & name, scalar speed);
			virtual ~PlayerController();
			
			void beginEvent(int id);
			void endEvent(int id);
			void update(scalar dt);
			
			void linkLocally(TGen::Engine::Entity & entity);
			void addCamera(const std::string & name, TGen::Camera * camera);
			TGen::Camera * getCamera(const std::string & name) const;
			
		protected:
			scalar speed;
			bool checkEvent(int id);

		private:
			uint32 activeEvents[20];
			
			TGen::SceneNode * node;		// IMPL
			typedef std::map<std::string, TGen::Camera *> CameraMap;
			
			CameraMap cameras;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PLAYERCONTROLLER_H

