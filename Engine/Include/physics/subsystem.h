/*
 *  physicssubsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICS_SUBSYSTEM_H
#define _TGEN_ENGINE_PHYSICS_SUBSYSTEM_H

#include "../subsystem.h"
#include "ode/ode.h"

namespace TGen {
	class Vector3;
	
	namespace Engine {
		class Entity;
		class StandardLogs;
		class Filesystem;
		
		namespace Physics {
			class Body;
			class Joint;
			class Geom;
			
			class Subsystem : public TGen::Engine::Subsystem {
			public:
				Subsystem(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem);
				~Subsystem();
			
				TGen::Engine::Component * createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				TGen::Engine::ComponentRecipe * createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
				
				void link();
				void update(scalar delta);
				void addGeom(TGen::Engine::Physics::Geom * geom);
				void addBody(TGen::Engine::Physics::Body * body);
				dWorldID getWorldId();
				
				static void nearCallback(void * data, dGeomID o1, dGeomID o2);
				
			private:
				TGen::Engine::Physics::Body * createBody(const std::string & name, const TGen::PropertyTree & properties);
				TGen::Engine::Physics::Joint * createJoint(const std::string & name, const TGen::PropertyTree & properties);
				TGen::Engine::Physics::Geom * createGeom(const std::string & name, const TGen::PropertyTree & properties);
				dMass getMass(const TGen::PropertyTree & properties);
				
				void setGravity(const TGen::Vector3 & gravity);
				
				std::vector<TGen::Engine::Physics::Body *> bodies;
				std::vector<TGen::Engine::Physics::Geom *> geoms;
				
				float updateInterval;
				double lastUpdate;
			
				TGen::Engine::StandardLogs & logs;
				
				dWorldID worldId;
				dSpaceID mainSpace;
				static dJointGroupID contactGroup;		// TODO: why???????
				
				TGen::Engine::Filesystem & filesystem;
			};
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICSSUBSYSTEM_H
