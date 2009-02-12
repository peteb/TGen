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
#include "worldobject.h"
#include "componentlink.h"

#include <tgen_math.h>
#include <ode/ode.h>

namespace TGen {
	namespace Engine {
		class Entity;
		
		namespace Scene {
			class Node;
		}
		
		namespace Physics {
			class Subsystem;
		
			class Body : public TGen::Engine::Component, public TGen::Engine::WorldObject {
			public:
				Body(const std::string & name, dBodyID bodyId, dWorldID worldId, dSpaceID spaceId);
				~Body();
				
				void preStep();
				void postStep();

				void link(const TGen::Engine::ComponentLinker & linker);
				
				dBodyID getBodyId() const;

				void setPosition(const TGen::Vector3 & position);
				TGen::Vector3 getPosition() const;
				
				void setOrientation(const TGen::Rotation & orientation);
				TGen::Rotation getOrientation() const;
				
				dWorldID getWorldId();
				dSpaceID getSpaceId();
				
				void setTurnHeadwise(bool turnHeadwise);
				void setMaxAngularSpeed(scalar speed);
				void setLink(const std::string & linkName);
				void setLink(TGen::Engine::WorldObject * linkedTo);
				void setKillTorque(bool killTorque);
				void setLinearDamping(scalar damping);
				void setFakeGravity(scalar fakeGrav);
				void setApplyGravity(bool applyGravity);
				void setAutoDisable(bool autoDisable);
				
				void setGroundNormal(const TGen::Vector3 & groundNormal);
				const TGen::Vector3 & getGroundNormal() const;
				
				void addForce(const TGen::Vector3 & force);
				void addForceWorld(const TGen::Vector3 & absPos, const TGen::Vector3 & force);
				
				void resetForces();
				void setEnabled(bool enabled);
				
				TGen::Vector3 getLinearVelocity() const;
				TGen::Vector3 getForce() const;
				
				// world object interface
				TGen::Vector3 getVelocity() const;
				
				
				void setOnFloor(bool onFloor);
				bool isOnFloor() const;
				
				
				void setSlope(scalar slope);
				float getSlope() const;
				
				scalar getGroundDefinition() const;
				scalar getMass() const;
				
			private:
				void updateFromScene();
				void updateScene();
				
				dBodyID bodyId;
				dWorldID worldId;
				dSpaceID spaceId;
				
				TGen::Vector3 groundNormal, linearVelocity, lastPosition;
				TGen::Engine::UnaryDelegate<TGen::Engine::WorldObject> delegate;
				bool turnHeadwise, killTorque;
				bool onFloor, doUpdateFromScene;
				scalar slope, fakeGrav;
				
			};
		} // !Physics
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_PHYSICS_BODY_H
