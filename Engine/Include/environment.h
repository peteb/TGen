/*
 *  environment.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/8/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ENVIRONMENT_H
#define _TGEN_ENGINE_ENVIRONMENT_H

#include <tgen_core.h>

namespace TGen {
	class Renderer;
	
	namespace Engine {
		class App;
		
		class Environment {
		public:
			Environment() {}
			virtual ~Environment() {}
			
			virtual int run(TGen::Engine::App * app) abstract;
			virtual TGen::Renderer & getRenderer() abstract;
			virtual void swapBuffers() abstract;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ENVIRONMENT_H
