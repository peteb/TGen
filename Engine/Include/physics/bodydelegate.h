/*
 *  bodydelegate.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICS_BODYDELEGATE_H
#define _TGEN_ENGINE_PHYSICS_BODYDELEGATE_H

#include "componentlink.h"
#include "physics/body.h"
#include "componentinterfaces.h"

namespace TGen {
	namespace Engine {
		namespace Physics {
			class BodyDelegate : public TGen::Engine::ComponentLink, public TGen::Engine::WorldObject {
			public:
				BodyDelegate() : bodyObject(NULL), worldObject(NULL) {}
				
				void link(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {
					bodyObject = dynamic_cast<TGen::Engine::Physics::Body *>(getComponent(entities, entity));
					worldObject = dynamic_cast<TGen::Engine::WorldObject *>(getComponent(entities, entity));
					
					if (bodyObject)
						worldObject = NULL;
				}
				
				void set(TGen::Engine::Physics::Body * link) {
					bodyObject = link;
				}
				
				void set(TGen::Engine::WorldObject * link) {
					worldObject = link;
				}

				void set(const std::string & name) {
					TGen::Engine::ComponentLink::set(name);
				}
				
				TGen::Engine::WorldObject * getWorldObject() const {
					return worldObject;
				}
				
				TGen::Engine::Physics::Body * getBodyObject() const {
					return bodyObject;
				}
				
				// interfaces
				TGen::Vector3 getPosition() const {
					if (worldObject)
						return worldObject->getPosition();
					
					return TGen::Vector3::Zero;
				}
				
				TGen::Rotation getOrientation() const {
					if (worldObject)
						return worldObject->getOrientation();
					
					return TGen::Rotation::Identity;
				}
				
				TGen::Vector3 getVelocity() const {
					if (worldObject)
						return worldObject->getVelocity();
					
					return TGen::Vector3::Zero;
				}
				
				void setPosition(const TGen::Vector3 & pos) {
					if (worldObject)
						worldObject->setPosition(pos);
				}
				
				void setOrientation(const TGen::Rotation & orientation) {
					if (worldObject)
						worldObject->setOrientation(orientation);
				}
				
				
			private:
				TGen::Engine::Physics::Body * bodyObject;
				TGen::Engine::WorldObject * worldObject;
			};
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICS_BODYDELEGATE_H

