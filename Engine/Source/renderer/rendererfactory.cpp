/*
 *  rendererfactory.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/22/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "renderer/rendererfactory.h"
#include "renderer/deferred/renderer.h"
#include "renderer/newrenderer/renderer.h"
#include <tgen_core.h>
#include "log.h"

TGen::Engine::WorldRendererFactory::WorldRendererFactory(TGen::Engine::StandardLogs & logs) 
	: logs(logs)
{
	
}

TGen::Engine::WorldRenderer * TGen::Engine::WorldRendererFactory::createRenderer(const std::string & name, TGen::Renderer & renderer, TGen::Engine::StandardLogs & logs, TGen::Engine::VariableRegister & variables, TGen::Engine::ResourceManager & resources) {
	
	logs.info["rfac"] << "creating renderer '" << name << "'..." << TGen::endl;
	
	if (name == "deferred")
		return new TGen::Engine::DeferredRenderer(renderer, logs, variables, resources);
	else if (name == "new")
		return new TGen::Engine::NewRenderer(renderer);
	
	throw TGen::RuntimeException("WorldRendererFactory::createRenderer", "invalid renderer name: '" + name + "'");
}
