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

namespace TGen {
	namespace Engine {
		class Entity;
		class StandardLogs;
		class BodyComponent;
		
		class PhysicsSubsystem : public TGen::Engine::Subsystem {
		public:
			PhysicsSubsystem(TGen::Engine::StandardLogs & logs);
			~PhysicsSubsystem();
			
			TGen::Engine::Component * createComponent(TGen::Engine::Entity * entity, const TGen::PropertyTree & properties);
			void link();
			void update(scalar delta);
			
		private:
			TGen::Engine::BodyComponent * createBody(const TGen::PropertyTree & properties);
			
			std::vector<TGen::Engine::BodyComponent *> bodies;
			
			TGen::Engine::StandardLogs & logs;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_PHYSICSSUBSYSTEM_H
