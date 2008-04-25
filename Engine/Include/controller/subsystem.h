/*
 *  subsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 4/15/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_CONTROLLER_SUBSYSTEM_H
#define _TGEN_ENGINE_CONTROLLER_SUBSYSTEM_H

#include "../subsystem.h"

namespace TGen {
	namespace Engine {
		class PlayerController;
		class Component;
		
		namespace Controller {
			class Subsystem : public TGen::Engine::Subsystem {
			public:
				Subsystem();
				~Subsystem();
				
				TGen::Engine::Component * createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties);				
				TGen::Engine::PlayerController * getController(const std::string & name);
				void update(scalar dt);
				
			private:
				typedef std::map<std::string, TGen::Engine::PlayerController *> ControllerMap;
				ControllerMap controllers;
			};
			
		} // !Controller
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_CONTROLLER_SUBSYSTEM_H

