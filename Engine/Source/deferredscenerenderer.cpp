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
#include "variablesregistry.h"
#include <tgen_graphics.h>

TGen::Engine::DeferredSceneRenderer::DeferredSceneRenderer(TGen::Engine::App & app) 
	: app(app)
{
	app.logs.info["dfr+"] << "deferred renderer initializing..." << TGen::endl;
	
	rhwNoTransformShader = app.globalResources.getShaderProgram("rhwNoTransform");
	screenFillMesh = app.globalResources.getMesh("gen:fillquad");
	screenFillMaterial = app.globalResources.getMaterial("deferred/screenFill");

	TGen::Rectangle mapSize(int(app.variables["env_width"]), int(app.variables["env_height"]));
	
	try {
		colorMap = depthMap = normalMap = NULL;
		mapTargets = NULL;
		createResources(mapSize);
		app.logs.info["dfr+"] << "we've got MRTs with the same size as the window. nice!" << TGen::endl;
	}
	catch (const TGen::RuntimeException & e) {	// trying power-of-two texture size
		delete colorMap; delete depthMap; delete normalMap; delete mapTargets;
		colorMap = depthMap = normalMap = NULL;
		mapTargets = NULL;
		
		mapSize = TGen::Rectangle(ceilPowerOfTwo(mapSize.width), ceilPowerOfTwo(mapSize.height));

		app.logs.warning["dfr+"] << e << TGen::endl;
		app.logs.warning["dfr+"] << "trying " << std::string(mapSize) << "..." << TGen::endl;
		
		createResources(mapSize);
	}
		
	app.logs.info["dfr+"] << "initialized" << TGen::endl;
}

TGen::Engine::DeferredSceneRenderer::~DeferredSceneRenderer() {
	delete mapTargets;
	
	delete colorMap;
	delete depthMap;
	delete normalMap;

	app.logs.info["dfr-"] << "shut down" << TGen::endl;
}

void TGen::Engine::DeferredSceneRenderer::createResources(const TGen::Rectangle & mapSize) {
	colorMap = app.renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	depthMap = app.renderer.createTexture(mapSize, TGen::DEPTH, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);	// TODO: ubyte på depth? wtf?
	normalMap = app.renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	
	mapTargets = app.renderer.createFrameBuffer();
	mapTargets->attachColor(colorMap);
	mapTargets->attachColor(normalMap);
	mapTargets->attachDepth(depthMap);	
}

void TGen::Engine::DeferredSceneRenderer::renderScene() {
	app.renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);
	
	// world har scene graph
	// ha en pekare till aktiv first person-kamera, gettas från world. typ world.getCamera("player_cam") 
	// input hit: geoms (map, entities, etc), camera, lights
	
	// det finns bara en screen-cam, men det är en pekare till någon kamera i världen
	// sen finns det kameror som rendrerar till texturer, men de hanteras åt andra sättet, dvs
	// ett material kopplas till en kamera
	// dvs textursources måste bli mer avancerade i materialsystemet, ska ju kunna ha envmap som uppdateras i realtid
	// sen borde man kunna definiera de kameror som binds till ett material också, hur ofta de ska uppdateras, osv
	
	// vad hanterar map och fiender osv? map? currentMap->fillRenderList(thisList);  
	// World!!!... ska det var ändå    var är camera? kom ihåg att camera borde hanteras av en playermovement-klass och att kameror ska kunna vara portabla
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

int TGen::Engine::DeferredSceneRenderer::ceilPowerOfTwo(int value) {
	int power = 2;
	while (value > power) {
		power *= 2;
	}
	
	return power;
}
