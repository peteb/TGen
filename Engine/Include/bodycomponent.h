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

namespace TGen {
	namespace Engine {
		class PhysicsSubsystem;
		class Entity;
		class SceneNodeComponent;
		
		class BodyComponent : public TGen::Engine::Component {
		public:
			BodyComponent(const std::string & name, float mass, const TGen::Vector3 & position);
			~BodyComponent();
			
			void update(float dt);
			void link(TGen::Engine::Entity & entity);
			
			friend class TGen::Engine::PhysicsSubsystem;
			
		private:
			float mass;
			TGen::Vector3 position;

			TGen::Engine::SceneNodeComponent * sceneNodeComponent;
		};
		
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_BODYCOMPONENT_H
