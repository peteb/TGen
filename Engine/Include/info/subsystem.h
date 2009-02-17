/*
 *  subsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_INFO_SUBSYSTEM_H
#define _TGEN_ENGINE_INFO_SUBSYSTEM_H

#include "../subsystem.h"

namespace TGen {
	namespace Engine {
		class World;
		
		namespace Info {
			class WorldInfo;
			
			class Subsystem : public TGen::Engine::Subsystem {
			public:
				Subsystem();
				
				TGen::Engine::Component * createComponent(const std::string & name, TGen::Engine::Entity & entity, const TGen::PropertyTree & properties); 	
				
				void setWorld(TGen::Engine::World * world);
				
			private:
				TGen::Engine::Info::WorldInfo * createWorldInfo(const std::string & name, const TGen::PropertyTree & properties);

				TGen::Engine::World * world;
			};
			
		} // !Info
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_INFO_SUBSYSTEM_H

