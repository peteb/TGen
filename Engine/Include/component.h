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

namespace TGen {
	namespace Engine {
		class Entity;
		class EntityList;
		
		class Component {
		public:
			Component(const std::string & name)
				: name(name) {}
			virtual ~Component() {}
			
			virtual void linkLocally(TGen::Engine::Entity & entity) {}
			virtual void linkGlobally(TGen::Engine::EntityList & entities, TGen::Engine::Entity & entity) {}

			const std::string & getName() const {return name; }
			
		protected:
			const std::string name;
		};
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_COMPONENT_H
