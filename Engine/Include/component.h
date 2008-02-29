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
		
		class Component {
		public:
			Component(const std::string & name)
				: name(name) {}
			virtual ~Component() {}
			
			std::string getName() const {return name; }
			
			virtual void link(TGen::Engine::Entity & entity) {}
			
		protected:
			const std::string name;
		};
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_COMPONENT_H
