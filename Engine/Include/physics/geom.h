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

namespace TGen {
	class Vector3;
	class Matrix3x3;
	
	namespace Engine {
		namespace Scene {
			class Node;
		}
		
		namespace Physics {
			class Body;
			
			class Geom : public TGen::Engine::Component {
			public:
				Geom(const std::string & name, const std::string & bodyComponent = "physBody");
				virtual ~Geom();
				
				float getFriction() const;
				void setFriction(float friction);
				void setBodyComponent(const std::string & body);
				void setAffectsOthers(bool affectOthers);
				bool getAffectsOthers() const;
				
				virtual void preStep();
				virtual void postStep();
				
				virtual bool onCollision(TGen::Engine::Physics::Geom * with, dGeomID id, const dContact & contactInfo) {return true; }
				
				virtual void linkLocally(TGen::Engine::Entity & entity);
				virtual void linkGlobally(TGen::Engine::EntityList & entities);
				
			protected:
				TGen::Engine::Physics::Body * attachedTo;
				
				void setGeomId(dGeomID id);
				void setPosition(const TGen::Vector3 & position);				
				void setOrientation(const TGen::Matrix3x3 & orientation);
				dGeomID geomId;
				std::string bodyComponent;
				
			private:				
				TGen::Engine::Scene::Node * sceneNodeComponent;
				bool affectsOthers;
				float friction;
			};
		} // !Physics
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICS_GEOM_H

