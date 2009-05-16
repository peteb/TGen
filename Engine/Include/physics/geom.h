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
#include "componentlink.h"
#include "physics/bodydelegate.h"

namespace TGen {
	class Vector3;
	class Matrix3x3;
	
	namespace Engine {
		class WorldObject;
		class Triggerable;
		class Entity;
		
		namespace Physics {
			class Body;
			class GeomScript;
			
			class Geom : public TGen::Engine::Component, public TGen::Engine::WorldObject {
			public:
				Geom(const std::string & name);
				virtual ~Geom();
				
				float getFriction() const;
				void setFriction(float friction);
				void setLink(const std::string & linkName);
				void setLink(TGen::Engine::WorldObject * linkedTo);
				void setBody(TGen::Engine::Physics::Body * body);
				
				void setAffectsOthers(bool affectOthers);
				bool getAffectsOthers() const;
				
				void updateClock(scalar dt);
				
				TGen::Engine::Physics::Body * getBody();
				TGen::Engine::WorldObject * getWorldLink();
				
				virtual void preStep();
				virtual void postStep();
				
				virtual bool onCollision(TGen::Engine::Physics::Geom * with, dGeomID id, const dContact & contactInfo);
				virtual void postCollision(TGen::Engine::Physics::Geom * with, dGeomID id, const dContact & contactInfo);
				virtual void onCollisionForce(scalar force, bool groundCollision, TGen::Engine::Physics::Geom * with);
				
				virtual void link(const TGen::Engine::ComponentLinker & linker);

				void setGeomId(dGeomID id);
				void setCategory(uint category);
				void setCollidesWith(uint collidesWith);
				uint getCategory() const;
				void setEnabled(bool enabled);
								
				scalar collisionForceThreshold, collisionForceScale;

				void setPosition(const TGen::Vector3 & position);				
				TGen::Vector3 getPosition() const;
				TGen::Rotation getOrientation() const;			
				TGen::Vector3 getVelocity() const;
				
				virtual void setOrientation(const TGen::Matrix3x3 & orientation);
				
				
				void setScriptInterface(TGen::Engine::Physics::GeomScript * scriptInterface);
				TGen::Engine::Physics::GeomScript * getScriptInterface() const;
				
			protected:
				
				dGeomID geomId;
				
			private:	
				void updateFromLink();
				void sendToLink();
				
				scalar time;
				bool affectsOthers;
				float friction;
				uint categoryBits, collidesWith;
				
				typedef std::map<Geom *, float> ColliderMap;
				ColliderMap lastColliders;
				
				TGen::Engine::Physics::BodyDelegate bodyDelegate;
				TGen::Engine::Physics::GeomScript * scriptInterface;
			};
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICS_GEOM_H

