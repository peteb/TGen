/*
 *  subsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 6/10/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_INVENTORY_SUBSYSTEM_H
#define _TGEN_ENGINE_INVENTORY_SUBSYSTEM_H

#include "../subsystem.h"

namespace TGen {
	namespace Engine {
		namespace Inventory {
			class Subsystem : public TGen::Engine::Subsystem {
			public:
				Subsystem();
				~Subsystem();
				
				TGen::Engine::Component * createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);
			}; 
		} // !Inventory
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_INVENTORY_SUBSYSTEM_H

