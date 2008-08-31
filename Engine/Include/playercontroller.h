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
#include "worldobject.h"
#include "symbols.h"
#include "componentlink.h"

namespace TGen {
	class SceneNode;
	
	namespace Engine {
		enum EventFlags {
			EventTriggered =	0x1000,
			EventKilled =		0x2000,
			EventRead =			0x4000,
		};
		
		namespace Scene {
			class Node;
		}
		
		class PlayerController : public TGen::Engine::Component, public TGen::Engine::WorldObject {
		public:
			PlayerController(const std::string & name);
			virtual ~PlayerController();
			
			virtual void link(const TGen::Engine::ComponentLinker & linker);
			virtual void update(scalar dt) abstract;
			virtual bool useRelativeView() const {return true; }
			virtual void trigger(TGen::Engine::TriggerContext & context, TriggerMode mode);
			
			void beginEvent(int id);
			void endEvent(int id);
			void addViewDelta(const TGen::Vector3 & view);

			//void addCamera(const std::string & name, const std::string & camera);
			//TGen::Engine::Scene::Node * getCamera(const std::string & name) const;
			void setCamera(const std::string & name);
			TGen::Engine::Scene::Node * getActiveCamera() const;
			
			// world object interface
			virtual TGen::Vector3 getVelocity() const;
			virtual TGen::Vector3 getPosition() const;
			virtual TGen::Rotation getOrientation() const;		
			virtual void setPosition(const TGen::Vector3 & pos) {}
			virtual void setOrientation(const TGen::Rotation & orientation) {}
			
			
		protected:
			bool isEventInitial(int id) const;
			bool checkEvent(int id);
			void setEventRead(int id);
			void resetEvents();
			
			TGen::Vector3 checkViewDelta();
			TGen::Vector3 checkViewAbs();
			bool ignoreInput;
			
		private:
			
			uint32 activeEvents[20];
			
			TGen::Vector3 viewDelta, viewAbs;
			
			TGen::Engine::UnaryDelegate<TGen::Engine::Scene::Node> camera;
			
			static TGen::Engine::Symbol symbolSetIgnoreInput;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PLAYERCONTROLLER_H

