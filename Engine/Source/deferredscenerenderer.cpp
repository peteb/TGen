/*
 *  deferredscenerenderer.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/31/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "deferredscenerenderer.h"
#include <tgen_graphics.h>

TGen::Engine::DeferredSceneRenderer::DeferredSceneRenderer(TGen::Renderer & renderer) 
	: renderer(renderer)
{
	
}

TGen::Engine::DeferredSceneRenderer::~DeferredSceneRenderer() {
	
}

void TGen::Engine::DeferredSceneRenderer::renderScene() {
	renderer.clearBuffers(TGen::ColorBuffer);
	
}

