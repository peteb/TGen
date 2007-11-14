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
	app.logs.info["dfr+"] << "deferred renderer initializing..." << TGen::endl;
	
	rhwNoTransformShader = app.globalResources.getShaderProgram("rhwNoTransform");
	screenFillMesh = app.globalResources.getMesh("gen:fillquad");
	screenFillMaterial = app.globalResources.getMaterial("deferred/screenFill");

	TGen::Rectangle mapSize(512, 512);	// TODO: fixa från settings
	
	colorMap = app.renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	depthMap = app.renderer.createTexture(mapSize, TGen::DEPTH, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);	// TODO: ubyte på depth? wtf?
	normalMap = app.renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	
	mapTargets = app.renderer.createFrameBuffer();
	mapTargets->attach(colorMap, TGen::FramebufferAttachmentColor);
	mapTargets->attach(normalMap, TGen::FramebufferAttachmentColor);
	mapTargets->attach(depthMap, TGen::FramebufferAttachmentDepth);
	
	// mapTarget->attachColor, mapTarget->attachDepth, mapTarget->attachStencil
	// så ska det bli!!!!!!!
	
	app.logs.info["dfr+"] << "initialized" << TGen::endl;
}

TGen::Engine::DeferredSceneRenderer::~DeferredSceneRenderer() {
	delete mapTargets;
	
	delete colorMap;
	delete depthMap;
	delete normalMap;

	app.logs.info["dfr-"] << "shut down" << TGen::endl;
}

void TGen::Engine::DeferredSceneRenderer::renderScene() {
	app.renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);
	
	// get current camera, apply view and projection	* TGen::Renderer, scene graph
	// get render list from map								* TGen::Renderer
	// set frame buffer
	// set viewport
	// render
	renderFillQuad();
}

void TGen::Engine::DeferredSceneRenderer::renderFillQuad() {
	screenFillMaterial->render(app.renderer, *screenFillMesh, "default", 9, NULL);
}
