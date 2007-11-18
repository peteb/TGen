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
#include "world.h"
#include <tgen_graphics.h>

TGen::Engine::DeferredSceneRenderer::DeferredSceneRenderer(TGen::Engine::App & app, TGen::Engine::World & world) 
	: app(app)
	, world(world)
	, mainCamera(NULL)
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
	colorMap = app.renderer.createTexture(mapSize, TGen::RGB, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	normalMap = app.renderer.createTexture(mapSize, TGen::RGB, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	depthMap = app.renderer.createTexture(mapSize, TGen::DEPTH, TGen::TypeUnsignedShort, TGen::TextureNoMipmaps);	// TODO: ubyte på depth? wtf?
	
	mapTargets = app.renderer.createFrameBuffer();
	mapTargets->attachColor(colorMap);
	mapTargets->attachColor(normalMap);
	mapTargets->attachDepth(depthMap);
	
	mrtSize = mapSize;
}

void TGen::Engine::DeferredSceneRenderer::renderScene() {
	if (!mainCamera) {
		mainCamera = world.getCamera("maincam");
		
		if (!mainCamera)
			return;
	}
	
	// packa ihop alla ljus som använder samma material (och som har samma timer)
	// rendrera deras fillquads i något i en metod högre än render och byt ut lampor mellan. men hur får man den nivån?
	
	// sen finns det kameror som rendrerar till texturer, men de hanteras åt andra sättet, dvs
	// ett material kopplas till en kamera
	// dvs textursources måste bli mer avancerade i materialsystemet, ska ju kunna ha envmap som uppdateras i realtid
	// sen borde man kunna definiera de kameror som binds till ett material också, hur ofta de ska uppdateras, osv
	
	// vad hanterar map och fiender osv? map? currentMap->fillRenderList(thisList);  
	// World!!!... ska det var ändå    var är camera? kom ihåg att camera borde hanteras av en playermovement-klass och att kameror ska kunna vara portabla
	// lampor i portalbana fixas genom att ta lampor från alla synliga rum + anslutande rum (även de som inte syns alltså)
	// kan sen optimeras genom att kolla lightboxen
	
	world.prepareLists(mainCamera);
	TGen::RenderList & renderList = world.getRenderList();
	TGen::Engine::LightList & lightList = world.getLightList();
	
	renderList.sort(*mainCamera, "default");

	TGen::Rectangle viewport = app.renderer.getViewport();
	
	
	// update g-buffers
	app.renderer.setRenderTarget(mapTargets);
	app.renderer.setViewport(mrtSize);
	app.renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);
	
	renderList.render(app.renderer, *mainCamera, "default");
	
	
	// render result
	app.renderer.setRenderTarget(NULL);
	app.renderer.setViewport(viewport);
	app.renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);

	renderFillQuad();
}

void TGen::Engine::DeferredSceneRenderer::renderFillQuad() {
	TGen::Texture * textures[] = {NULL, colorMap, depthMap, normalMap};
	
	screenFillMaterial->render(app.renderer, *screenFillMesh, "default", 9, textures);
}

int TGen::Engine::DeferredSceneRenderer::ceilPowerOfTwo(int value) {
	int power = 2;
	while (value > power) {
		power *= 2;
	}
	
	return power;
}
