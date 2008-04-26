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
#include <tgen_math.h>
#include "component.h"

namespace TGen {
	class Camera;
	class SceneNode;
	
	namespace Engine {
		enum EventFlags {
			EventTriggered =	0x1000,
			EventKilled =		0x2000,
			EventRead =			0x4000,
		};
		
		
		class PlayerController : public TGen::Engine::Component {
		public:
			PlayerController(const std::string & name);
			virtual ~PlayerController();
			
			void addViewDelta(const TGen::Vector3 & view);
			
			void beginEvent(int id);
			void endEvent(int id);
			void addCamera(const std::string & name, const std::string & camera);
			TGen::Camera * getCamera(const std::string & name) const;
			
			virtual void linkLocally(TGen::Engine::Entity & entity);
			virtual void update(scalar dt) abstract;
			
		protected:
			bool checkEvent(int id);
			TGen::Vector3 checkViewDelta();
			
		private:
			void linkCameras(TGen::Engine::Entity & entity);
									
			typedef std::map<std::string, TGen::Camera *> CameraMap;
			typedef std::map<std::string, std::string> StringStringMap;
			
			StringStringMap camerasToLink;
			CameraMap cameras;

			uint32 activeEvents[20];
			
			TGen::Vector3 viewDelta;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PLAYERCONTROLLER_H

