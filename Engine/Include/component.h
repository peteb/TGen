/*
 *  component.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_COMPONENT_H
#define _TGEN_ENGINE_COMPONENT_H

#include <string>
#include "triggerable.h"

namespace TGen {
	namespace Engine {
		class Entity;
		class EntityList;
		
		class Component : public TGen::Engine::Triggerable {
		public:
			Component(const std::string & name, bool staticComponent = false);
			virtual ~Component();
			
			virtual void linkLocally(TGen::Engine::Entity & entity);
			virtual void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity);
			virtual void linkIndependently() {}
			virtual void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
			
			bool isStatic() const;
			const std::string & getName() const;
			void setOwner(TGen::Engine::Entity * owner);
			TGen::Engine::Entity * getOwner() const;
			
		protected:
			const std::string name;
			const bool staticComponent;
			TGen::Engine::Entity * owner;
		};
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_COMPONENT_H
