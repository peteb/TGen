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
{
	rhwNoTransformShader = app.globalResources.getShaderProgram("rhwNoTransform.shader");
	screenFillMesh = app.globalResources.getMesh("gen:fillquad");
	screenFillMaterial = app.globalResources.getMaterial("deferred/screenFill");
}

TGen::Engine::DeferredSceneRenderer::~DeferredSceneRenderer() {
	
}

void TGen::Engine::DeferredSceneRenderer::renderScene() {
	app.renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);
	
	renderFillQuad();
}

void TGen::Engine::DeferredSceneRenderer::renderFillQuad() {
	screenFillMaterial->render(app.renderer, *screenFillMesh, "default", 9, NULL);
}
