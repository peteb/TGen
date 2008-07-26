/*
 *  geom.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/12/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_PHYSICS_GEOM_H
#define _TGEN_ENGINE_PHYSICS_GEOM_H

#include "component.h"
#include <ode/ode.h>
#include <tgen_core.h>
#include "triggerable.h"
#include "componentlink.h"
#include "physics/bodydelegate.h"

namespace TGen {
	class Vector3;
	class Matrix3x3;
	
	namespace Engine {
		class WorldObject;
		class Triggerable;
		
		namespace Physics {
			class Body;
			
			class Geom : public TGen::Engine::Component {
			public:
				Geom(const std::string & name);
				virtual ~Geom();
				
				virtual void trigger(TGen::Engine::TriggerContext & context, TriggerMode mode);
				
				float getFriction() const;
				void setFriction(float friction);
				void setLink(const std::string & linkName);
				void setLink(TGen::Engine::WorldObject * linkedTo);
				void setBody(TGen::Engine::Physics::Body * body);
				
				void setAffectsOthers(bool affectOthers);
				bool getAffectsOthers() const;
				
				TGen::Engine::Physics::Body * getBody();
				
				virtual void preStep();
				virtual void postStep();
				
				virtual bool onCollision(TGen::Engine::Physics::Geom * with, dGeomID id, const dContact & contactInfo) {return true; }
				virtual void onCollisionForce(scalar force, bool groundCollision);
				
				virtual void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity);

				void setGeomId(dGeomID id);
				void setCategory(uint category);
				void setCollidesWith(uint collidesWith);
				uint getCategory() const;
				
				void setEventCollisionForce(const std::string & eventName);
				
				scalar collisionForceThreshold, collisionForceScale;

				
			protected:
				void setPosition(const TGen::Vector3 & position);				
				virtual void setOrientation(const TGen::Matrix3x3 & orientation);
				
				dGeomID geomId;
				
			private:	
				void updateFromLink();
				void sendToLink();
				

				bool affectsOthers;
				float friction;
				uint categoryBits, collidesWith;
				
				TGen::Engine::Physics::BodyDelegate link;
				TGen::Engine::UnaryDelegate<TGen::Engine::Triggerable> eventCollisionForce;
			};
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICS_GEOM_H

