/*
 *  worldrenderer.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/21/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "worldrenderer.h"

TGen::Engine::WorldRenderer::WorldRenderer(TGen::Renderer & renderer)
	: renderer(renderer)
{
}

TGen::Engine::WorldRenderer::~WorldRenderer() {
	
}

TGen::Renderer & TGen::Engine::WorldRenderer::getRenderer() {
	return renderer;
}
