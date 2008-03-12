/*
 *  physicssubsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICSSUBSYSTEM_H
#define _TGEN_ENGINE_PHYSICSSUBSYSTEM_H

#include "subsystem.h"
#include "ode/ode.h"

namespace TGen {
	class Vector3;
	
	namespace Engine {
		class Entity;
		class StandardLogs;
		class BodyComponent;
		class JointComponent;
		class PhysicsPropertiesComponent;
		class GeomComponent;
		
		class PhysicsSubsystem : public TGen::Engine::Subsystem {
		public:
			PhysicsSubsystem(TGen::Engine::StandardLogs & logs);
			~PhysicsSubsystem();
			
			TGen::Engine::Component * createComponent(TGen::Engine::Entity * entity, const TGen::PropertyTree & properties);
			void link();
			void update(scalar delta);
			
			static void nearCallback(void * data, dGeomID o1, dGeomID o2);
			
		private:
			TGen::Engine::BodyComponent * createBody(const TGen::PropertyTree & properties);
			TGen::Engine::JointComponent * createJoint(const TGen::PropertyTree & properties);
			TGen::Engine::GeomComponent * createGeom(const TGen::PropertyTree & properties);
			TGen::Engine::PhysicsPropertiesComponent * createProps(const TGen::PropertyTree & properties);
			
			void setGravity(const TGen::Vector3 & gravity);
			
			std::vector<TGen::Engine::BodyComponent *> bodies;
			float updateInterval;
			
			TGen::Engine::StandardLogs & logs;
			
			static dWorldID worldId;
			dSpaceID mainSpace;
			static dJointGroupID contactGroup;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICSSUBSYSTEM_H
