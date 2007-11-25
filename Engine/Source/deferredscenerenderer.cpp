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

TGen::Engine::DeferredRenderer::DeferredRenderer(TGen::Engine::App & app, TGen::Engine::World & world) 
	: app(app)
	, vars(app)
	, world(world)
	, mainCamera(NULL)
	, lastNumLights(0)
{
	app.logs.info["dfr+"] << "deferred renderer initializing..." << TGen::endl;
	
	rhwNoTransformShader = app.globalResources.getShaderProgram("rhwNoTransform");
	rhwOnlyColorMaterial = app.globalResources.getMaterial("deferred/rhwOnlyColor");
	screenFillMesh = app.globalResources.getMesh("gen:fillquad");
	lightAmbientMaterial = app.globalResources.getMaterial("deferred/lightAmbient");
	lightDirectionalMaterial = app.globalResources.getMaterial("deferred/lightDirectional2");
	postLuminanceMaterial = app.globalResources.getMaterial("post/luminance");
	postGaussianHorizMaterial = app.globalResources.getMaterial("post/gaussianHoriz");
	postGaussianVertMaterial = app.globalResources.getMaterial("post/gaussianVert");
	postFinalBloom = app.globalResources.getMaterial("post/finalBloom");
	
	TGen::Rectangle mapSize(int(app.variables["env_width"]), int(app.variables["env_height"]));
	
	if (vars.forceBinaryMRT)
		mapSize = TGen::Rectangle(ceilPowerOfTwo(mapSize.width), ceilPowerOfTwo(mapSize.height));
	
	try {
		colorMap = depthMap = normalMap = miscMap = postMap1 = postMap2 = postMap3 = NULL;
		mapTargets = postTargets1 = postTargets2 = postTargets3 = NULL;
		
		createResources(mapSize);
	}
	catch (const TGen::RuntimeException & e) {	// trying power-of-two texture size
		delete colorMap; delete depthMap; delete normalMap; delete miscMap; delete mapTargets; delete postMap1; delete postMap2; delete postTargets1; delete postTargets2; delete postMap3; delete postTargets3;
		colorMap = depthMap = normalMap = miscMap = postMap1 = postMap2 = postMap3 = NULL;
		mapTargets = postTargets1 = postTargets2 = postTargets3 = NULL;
		
		mapSize = TGen::Rectangle(ceilPowerOfTwo(mapSize.width), ceilPowerOfTwo(mapSize.height));

		app.logs.warning["dfr+"] << e << TGen::endl;
		app.logs.warning["dfr+"] << "trying " << std::string(mapSize) << "..." << TGen::endl;
		
		createResources(mapSize);
	}
	
	app.logs.info["dfr+"] << "mrts size: " << std::string(mapSize) << TGen::endl;
	// TODO: bloom wrappar
	
	TGen::Rectangle viewport = app.renderer.getViewport();
	app.renderer.setRenderTarget(postTargets2);
	app.renderer.setViewport(downsampleSize);
	app.renderer.clearBuffers(TGen::ColorBuffer);

	app.renderer.setRenderTarget(NULL);
	app.renderer.setViewport(viewport);
	
	app.logs.info["dfr+"] << "bloom downsampling: " << std::string(downsampleSize) << TGen::endl;
	app.logs.info["dfr+"] << "initialized" << TGen::endl;
}

TGen::Engine::DeferredRenderer::~DeferredRenderer() {
	delete mapTargets;
	delete postTargets1;
	delete postTargets2;
	delete postTargets3;
	
	delete colorMap;
	delete depthMap;
	delete normalMap;
	delete miscMap;
	delete postMap1;
	delete postMap2;
	delete postMap3;
	
	app.logs.info["dfr-"] << "shut down" << TGen::endl;
}

void TGen::Engine::DeferredRenderer::createResources(const TGen::Rectangle & mapSize) {
	downsampleSize = mapSize / scalar(pow(2.0f, vars.bloomDownsampling));
	
	colorMap = app.renderer.createTexture(mapSize, TGen::RGB, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	normalMap = app.renderer.createTexture(mapSize, TGen::RGB, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	miscMap = app.renderer.createTexture(mapSize, TGen::RGB, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	depthMap = app.renderer.createTexture(mapSize, TGen::DEPTH, TGen::TypeUnsignedShort, TGen::TextureNoMipmaps);	// TODO: ubyte på depth? wtf?
	
	postMap1 = app.renderer.createTexture(mapSize, TGen::RGB, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	postMap2 = app.renderer.createTexture(downsampleSize, TGen::RGB, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	postMap3 = app.renderer.createTexture(downsampleSize, TGen::RGB, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	
	/*colorMap->setFilterMode(TGen::TextureFilterNearest, TGen::TextureFilterNearest);
	normalMap->setFilterMode(TGen::TextureFilterNearest, TGen::TextureFilterNearest);
	depthMap->setFilterMode(TGen::TextureFilterNearest, TGen::TextureFilterNearest);
	*/
	
	mapTargets = app.renderer.createFrameBuffer();
	mapTargets->attachColor(colorMap);
	mapTargets->attachColor(normalMap);
	mapTargets->attachColor(miscMap);
	mapTargets->attachDepth(depthMap);
	
	postTargets1 = app.renderer.createFrameBuffer();
	postTargets1->attachColor(postMap1);
	
	postTargets2 = app.renderer.createFrameBuffer();
	postTargets2->attachColor(postMap2);
	
	postTargets3 = app.renderer.createFrameBuffer();
	postTargets3->attachColor(postMap3);
	
	mrtSize = mapSize;
}

void TGen::Engine::DeferredRenderer::renderScene(scalar dt) {
	if (!mainCamera) {
		mainCamera = world.getCamera("maincam");
		
		if (!mainCamera)
			return;
	}
	
	// TODO: early-z, z-pass first. ska kunna aktiveras/avaktiveras genom variabel
	
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
	// TODO: kolla vilket kordinatsystem glLightfv använder

	world.prepareLists(mainCamera);
	TGen::RenderList & renderList = world.getRenderList();
	TGen::Engine::LightList & lightList = world.getLightList();
	
	renderList.sort(*mainCamera, "default");

	TGen::Rectangle viewport = app.renderer.getViewport();
	
	
	// UPDATE MAPS
	app.renderer.setRenderTarget(mapTargets);
	app.renderer.setViewport(mrtSize);
	app.renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);
	app.renderer.setAmbientLight(world.getAmbientLight());
	
	
	renderList.render(app.renderer, *mainCamera, "default");
	
	if (vars.postProcessing) {
		app.renderer.setRenderTarget(postTargets1);
		app.renderer.setViewport(mrtSize);
	}
	else {
		app.renderer.setRenderTarget(NULL);
		app.renderer.setViewport(viewport);
	}
	

	// AMBIENT TO RESULT
	//app.renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);
	renderFillQuad(lightAmbientMaterial);
	

	// LIGHTING
	for (int i = 0; i < lightList.getNumLights(); i += 8) {
		//lastNumLights = 0;
		
		for (int a = 0; a < 8; ++a) {
			if (i + a < lightList.getNumLights()) {
				//std::cout << "set light " << a << " to " << i + a << " (" << std::string(lightList.getLight(i + a).specular) << std::endl;
				//lastNumLights++;
				app.renderer.setLight(a, lightList.getLight(i + a));
			}
		}	
		
		renderFillQuad(lightDirectionalMaterial);
	}
	

	if (vars.postProcessing) {
		postProcessing(viewport);		
	}

}

void TGen::Engine::DeferredRenderer::postProcessing(const TGen::Rectangle & viewport) {
	// TODO: flökar säkert sönder depthbuffern.. disabla
	// TODO: alphan som clearas med ska justeras beroende på fps
	
	app.renderer.setViewport(downsampleSize);
	app.renderer.setRenderTarget(postTargets2);
	
	if (vars.lumTrace) {
		app.renderer.setColor(TGen::Color(0.0, 0.0, 0.0, 0.3));
		renderFillQuad(rhwOnlyColorMaterial);
	}
	
	renderPostFillQuad(postLuminanceMaterial);
	
	for (int i = 0; i < vars.bloomBlurPasses; ++i) {
		app.renderer.setRenderTarget(postTargets3);	
		renderPost2FillQuad(postGaussianHorizMaterial);
	
		app.renderer.setRenderTarget(postTargets2);
		renderPost3FillQuad(postGaussianVertMaterial);
	}
	
	app.renderer.setRenderTarget(NULL);
	app.renderer.setViewport(viewport);
	
	renderPostFinalQuad(postFinalBloom);
}

void TGen::Engine::DeferredRenderer::renderFillQuad(TGen::Material * material) {
	if (!material || !screenFillMesh)
		throw TGen::RuntimeException("DeferredRenderer::renderFillQuad", "missing resources");

	TGen::Texture * textures[] = {NULL, colorMap, normalMap, miscMap, depthMap};
	
	material->render(app.renderer, *screenFillMesh, "default", 9, textures, this);
}

void TGen::Engine::DeferredRenderer::renderPostFillQuad(TGen::Material * material) {
	TGen::Texture * textures[] = {NULL, postMap1, normalMap, miscMap, depthMap};
	
	material->render(app.renderer, *screenFillMesh, "default", 9, textures, this);	
}

void TGen::Engine::DeferredRenderer::renderPost2FillQuad(TGen::Material * material) {
	TGen::Texture * textures[] = {NULL, postMap2, normalMap, miscMap, depthMap};
	
	material->render(app.renderer, *screenFillMesh, "default", 9, textures, this);	
}

void TGen::Engine::DeferredRenderer::renderPost3FillQuad(TGen::Material * material) {
	TGen::Texture * textures[] = {NULL, postMap3, normalMap, miscMap, depthMap};
	
	material->render(app.renderer, *screenFillMesh, "default", 9, textures, this);	
}

void TGen::Engine::DeferredRenderer::renderPostFinalQuad(TGen::Material * material) {
	TGen::Texture * textures[] = {NULL, postMap1, postMap2, miscMap, depthMap};
	
	material->render(app.renderer, *screenFillMesh, "default", 9, textures, this);		
}

void TGen::Engine::DeferredRenderer::updateShaderVariable(TGen::ShaderVariable & var, const std::string & name) {
	if (name == "$texelwidth") {
		var = scalar(1.0 / downsampleSize.width);
	}
	else if (name == "$texelheight") {
		var = scalar(1.0 / downsampleSize.height);
	}
	else if (name == "$lummin") {
		var = vars.lumMin;
	}
	else if (name == "$lummultiplier") {
		var = vars.lumMultiplier;
	}
	else if (name == "$lumkilltrace") {
		var = !vars.lumTrace;
	}
	else if (name == "$numlights") {
		var = lastNumLights;		
	}
	else {
		app.logs.warning["dfr"] << "nothing to bind for '" << name << "'!" << TGen::endl;
	}
}

// TODO: fixa simpel fps

int TGen::Engine::DeferredRenderer::ceilPowerOfTwo(int value) {
	int power = 2;
	while (value > power) {
		power *= 2;
	}
	
	return power;
}
