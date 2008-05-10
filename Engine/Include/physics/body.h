/*
 *  body.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/29/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICS_BODY_H
#define _TGEN_ENGINE_PHYSICS_BODY_H

#include "component.h"
#include <tgen_math.h>
#include <ode/ode.h>

// TGen::Engine::Physics::Subsystem    #include "physics/subsystem.h"
// TGen::Engine::Physics::BodyComponent
// TGen::Engine::Physics::PlaneGeom
// TGen::Engine::Physics::BoxGeom    #include "physics/boxgeom.h"

namespace TGen {
	namespace Engine {
		class Entity;
		
		namespace Scene {
			class Node;
		}
		
		namespace Physics {
			class Subsystem;
		
			class Body : public TGen::Engine::Component {
			public:
				Body(const std::string & name, dBodyID bodyId, dWorldID worldId, dSpaceID spaceId);
				~Body();
				
				void preStep();
				void postStep();
				void linkLocally(TGen::Engine::Entity & entity);
				dBodyID getBodyId() const;
				
				void setPosition(const TGen::Vector3 & position);
				TGen::Vector3 getPosition() const;
				
				void setOrientation(const TGen::Matrix3x3 & orientation);
				TGen::Matrix3x3 getOrientation() const;
				
				dWorldID getWorldId();
				dSpaceID getSpaceId();
				
				void setTurnHeadwise(bool turnHeadwise);
				void setMaxAngularSpeed(scalar speed);
				void setNodeComponent(const std::string & nodeName);
				void setKillTorque(bool killTorque);
				void setLinearDamping(scalar damping);
				void setFakeGravity(scalar fakeGrav);
				
				void addForce(const TGen::Vector3 & force);
				TGen::Vector3 getLinearVelocity() const;

				
				
				void setOnFloor(bool onFloor);
				bool isOnFloor() const;
				
				
				void setSlope(scalar slope);
				float getSlope() const;
				
			private:
				std::string nodeComponent;
				dBodyID bodyId;
				dWorldID worldId;
				dSpaceID spaceId;
				
				TGen::Engine::Scene::Node * sceneNodeComponent;
				bool turnHeadwise, killTorque;
				bool onFloor;
				scalar slope, fakeGrav;
			};
		} // !Physics
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_PHYSICS_BODY_H
