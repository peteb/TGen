/*
 *  deferredscenerenderer.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/31/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "deferredscenerenderer.h"
#include "app.h"
#include "filesystem.h"
#include "file.h"
#include <tgen_graphics.h>

TGen::Engine::DeferredSceneRenderer::DeferredSceneRenderer(TGen::Engine::App & app) 
	: app(app)
	, rhwNoTransformShader(NULL)
	, screenFillMesh(NULL)
{
	rhwNoTransformShader = app.globalResources.getShaderProgram("rhwNoTransform.shader");
	screenFillMesh = app.globalResources.getMesh("gen:fillquad");
}

TGen::Engine::DeferredSceneRenderer::~DeferredSceneRenderer() {
	
}

void TGen::Engine::DeferredSceneRenderer::renderScene() {
	app.renderer.clearBuffers(TGen::ColorBuffer);
	
}

