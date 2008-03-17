/*
 *  bodycomponent.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_BODYCOMPONENT_H
#define _TGEN_ENGINE_BODYCOMPONENT_H

#include "component.h"
#include <tgen_math.h>
#include <ode/ode.h>

// TGen::Engine::Physics::Subsystem    #include "physics/subsystem.h"
// TGen::Engine::Physics::BodyComponent
// TGen::Engine::Physics::PlaneGeom
// TGen::Engine::Physics::BoxGeom    #include "physics/boxgeom.h"

namespace TGen {
	namespace Engine {
		class PhysicsSubsystem;
		class Entity;
		class SceneNodeComponent;
		
		class BodyComponent : public TGen::Engine::Component {
		public:
			BodyComponent(const std::string & name, dBodyID bodyId);
			~BodyComponent();
			
			void preStep();
			void postStep();
			void linkLocally(TGen::Engine::Entity & entity);
			dBodyID getBodyId() const;
			
			void setPosition(const TGen::Vector3 & position);
			TGen::Vector3 getPosition() const;
			
			void setOrientation(const TGen::Matrix3x3 & orientation);
			TGen::Matrix3x3 getOrientation() const;
			
		private:
	//		float mass;
		//	TGen::Vector3 position;
			
			dBodyID bodyId;
			TGen::Engine::SceneNodeComponent * sceneNodeComponent;
		};
		
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_BODYCOMPONENT_H
