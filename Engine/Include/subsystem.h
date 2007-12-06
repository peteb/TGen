/*
 *  subsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SUBSYSTEM_H
#define _TGEN_ENGINE_SUBSYSTEM_H

#include <tgen_core.h>

namespace TGen {
	namespace Engine {
		class Component;
		class Entity;
		
		class Subsystem {
		public:
			virtual ~Subsystem() {}
			
			virtual TGen::Engine::Component * createComponent(TGen::Engine::Entity * entity, const TGen::PropertyTree & properties) abstract;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SUBSYSTEM_H
