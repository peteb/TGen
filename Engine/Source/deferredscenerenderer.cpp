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
{
	rhwNoTransformShader = loadShader("rhwNoTransform.shader");
}

TGen::Engine::DeferredSceneRenderer::~DeferredSceneRenderer() {
	delete rhwNoTransformShader;
}

void TGen::Engine::DeferredSceneRenderer::renderScene() {
	app.renderer.clearBuffers(TGen::ColorBuffer);
	
}

TGen::ShaderProgram * TGen::Engine::DeferredSceneRenderer::loadShader(const std::string & filename) {
	app.logs.info["dfre"] << "loading shader " << filename << "..." << TGen::endl;
	
	TGen::Engine::File * file = app.filesystem.openRead("/shaders/" + filename);
	std::string contents = file->readAll();
	delete file;
	
	TGen::ShaderProgram * program = app.renderer.createShaderProgram(contents.c_str());
	program->link();
	return program;
}
