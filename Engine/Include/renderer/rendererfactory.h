/*
 *  rendererfactory.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/22/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_RENDERER_RENDERERFACTORY_H
#define _TGEN_ENGINE_RENDERER_RENDERERFACTORY_H

#include <string>

namespace TGen {
	class Renderer;
	
	namespace Engine {
		class WorldRenderer;
		class StandardLogs;
		class VariableRegister;
		class ResourceManager;
		
		class WorldRendererFactory {
		public:
			WorldRendererFactory(TGen::Engine::StandardLogs & logs);
			
			TGen::Engine::WorldRenderer * createRenderer(const std::string & name, TGen::Renderer & renderer, TGen::Engine::StandardLogs & logs, TGen::Engine::VariableRegister & variables, TGen::Engine::ResourceManager & resources);
			
		private:
			TGen::Engine::StandardLogs & logs;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_RENDERERFACTORY_H

