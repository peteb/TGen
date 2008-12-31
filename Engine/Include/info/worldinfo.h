/*
 *  worldinfo.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 7/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_WORLDINFO_H
#define _TGEN_ENGINE_WORLDINFO_H

#include "component.h"
#include "componentlink.h"
#include "symbols.h"
#include "color.h"

namespace TGen {
	namespace Engine {
		class EntityList;
		class Entity;
		class PlayerController;
		class World;
		
		namespace Info {
			class WorldInfo : public TGen::Engine::Component {
			public:
				WorldInfo(const std::string & name, TGen::Engine::World * world);
				
				void link(const TGen::Engine::ComponentLinker & linker);
				void trigger(TGen::Engine::TriggerContext & context, TGen::Engine::TriggerMode mode);
				
				void setPlayerController(const std::string & name);
				TGen::Engine::PlayerController * getPlayerController();
				
				void setAmbientLight(const TGen::Color & light);
				TGen::Color getAmbientLight() const;
				
			private:
				TGen::Engine::UnaryDelegate<TGen::Engine::PlayerController> playerController;
				
				static TGen::Engine::Symbol symbolSetGravity, symbolGetGravity;
				TGen::Engine::World * world;
				TGen::Color ambientLight;
			};
			
		} // !Info
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_WORLDINFO_H

