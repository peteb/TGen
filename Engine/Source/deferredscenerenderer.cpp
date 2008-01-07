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
#include "light.h"
#include "vbrenderable.h"
#include <tgen_graphics.h>

TGen::Engine::DeferredRenderer::DeferredRenderer(TGen::Engine::App & app) 
	: app(app)
	, vars(app)
	, mainCamera(NULL)
	, lastNumLights(0)
	, lightBatchSize(8)
	, lightMaterials(NULL)
	, world(NULL)
{
	app.logs.info["dfr+"] << "deferred renderer initializing..." << TGen::endl;
	
	rhwNoTransformShader = app.globalResources.getShaderProgram("rhwNoTransform");
	rhwOnlyColorMaterial = app.globalResources.getMaterial("deferred/rhwOnlyColor");
	screenFillMesh = app.globalResources.getMesh("gen:fillquad");
	lightAmbientMaterial = app.globalResources.getMaterial("deferred/lightAmbient");
	lightDirectionalMaterial = app.globalResources.getMaterial("deferred/lightDirectional");
	lightPositionalMaterial = app.globalResources.getMaterial("deferred/lightPositional");
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
	
	
	lightMaterials = new TGen::Material*[lightBatchSize * 3];
	//loadLightMaterial("deferred/lightDirectional", 0);
	
	app.logs.info["dfr+"] << "light batch size: " << lightBatchSize << TGen::endl;
	
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
	app.logs.info["dfr-"] << "shutting down..." << TGen::endl;

	delete [] lightMaterials;
	
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
	
}

void TGen::Engine::DeferredRenderer::loadLightMaterial(const std::string & name, int materialId) {
	for (int i = 0; i < lightBatchSize; ++i)
		lightMaterials[materialId * lightBatchSize + i] = app.globalResources.getMaterial(name + TGen::lexical_cast<std::string>(i));
}

void TGen::Engine::DeferredRenderer::createResources(const TGen::Rectangle & mapSize) {
	downsampleSize = mapSize / scalar(pow(2.0f, vars.bloomDownsampling));
	
	colorMap = app.renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	normalMap = app.renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	miscMap = app.renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	depthMap = app.renderer.createTexture(mapSize, TGen::DEPTH24, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps /*| TGen::TextureRectangle*/);	// TODO: ubyte på depth? wtf?
	
	postMap1 = app.renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);		// <-- sparar en massa skit... varför? blendfunc?
	postMap2 = app.renderer.createTexture(downsampleSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	postMap3 = app.renderer.createTexture(downsampleSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	
	postMap1->setWrapMode(TGen::TextureWrapClamp, TGen::TextureWrapClamp);
	postMap2->setWrapMode(TGen::TextureWrapClamp, TGen::TextureWrapClamp);
	postMap3->setWrapMode(TGen::TextureWrapClamp, TGen::TextureWrapClamp);

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
	//postTargets1->attachDepth(depthMap);
	
	postTargets2 = app.renderer.createFrameBuffer();
	postTargets2->attachColor(postMap2);
	
	postTargets3 = app.renderer.createFrameBuffer();
	postTargets3->attachColor(postMap3);
	
	//depthTarget = app.renderer.createFrameBuffer();
	//depthTarget->attachDepth(depthMap);
	
	mrtSize = mapSize;
}

void TGen::Engine::DeferredRenderer::renderScene(scalar dt) {
	if (!world) {
		renderWorldless(dt);
		return;
	}
	
	if (!mainCamera) {
		mainCamera = world->getCamera("maincam");
		
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

	world->prepareLists(mainCamera);
	TGen::RenderList & renderList = world->getRenderList();
	TGen::Engine::LightList & lightList = world->getLightList();
	
	renderList.sort(*mainCamera, "default");

	TGen::Rectangle viewport = app.renderer.getViewport();
	
	
	// UPDATE MAPS (color, normal, spec, depth, etc)
	app.renderer.setRenderTarget(mapTargets);
	app.renderer.setViewport(mrtSize);
	app.renderer.setClearColor(TGen::Color::Black);
	app.renderer.clearBuffers(/*TGen::ColorBuffer | */TGen::DepthBuffer);
	app.renderer.setAmbientLight(world->getAmbientLight());
	
	renderList.render(app.renderer, *mainCamera, "default");
	
	vars.postProcessing = false;
	// postprocessing kostar 110 fps
	
	if (vars.postProcessing) {
		app.renderer.setRenderTarget(postTargets1);
		app.renderer.setViewport(mrtSize);
	}
	else {
		app.renderer.setRenderTarget(NULL);
		app.renderer.setViewport(viewport);
	}
	
	//app.renderer.clearBuffers(TGen::DepthBuffer);

	// AMBIENT TO RESULT
	renderFillQuad(lightAmbientMaterial);
	
	// LIGHTS TO RESULT
	TGen::Engine::LightList::LightMap & lightsByMaterial = lightList.getLightsByMaterial();
	for (TGen::Engine::LightList::LightMap::iterator iter = lightsByMaterial.begin(); iter != lightsByMaterial.end(); ++iter) {
		//std::cout << "MATERIAL " << iter->first << std::endl;
		
		// TODO: OM EN LAMPA HAR FACES SÅ RENDRERA DEM ist för fillquad!!!! det är bounding boxes
		//       om man är innanför en bbox för ett ljus rita backfaces, annars bara frontfaces
		//       för mindre ljus kan man köra bbox. kolla mot bounding-sphere eller aabb.
		
		TGen::Engine::LightList::LightArray * lights = iter->second;
		if (lights) {
			for (int i = 0; i < lights->size(); i += lightBatchSize) {
				int a = 0;
				for (; a < lightBatchSize && i + a < lights->size(); ++a) {
					app.renderer.setTransform(TGen::TransformWorldView, (*lights)[a]->getTransform());
					app.renderer.setLight(a, (*lights)[a]->getLightProperties());
				}
				
				renderFillQuad(iter->first, TGen::lexical_cast<std::string>(a) + "lights");	// TODO: optimize
			}
		}
	}
	
	//}
	//lightType = 1;	
	/*TGen::VertexBuffer * vb = app.renderer.createVertexBuffer(LightVertexDecl(), sizeof(LightVertexDecl::Type) * 24 * 8, TGen::UsageStream);
	
	for (int i = 0; i < lights[lightType].size(); i += lightBatchSize) {
		std::vector<LightVertexDecl::Type> vertices;
		vertices.reserve(24 * 8);
		
		int a = 0;
		for (; a < lightBatchSize && i + a < lights[lightType].size(); ++a) {
			TGen::Vector3 min = lights[lightType][i + a]->boundingBox.getMin();
			TGen::Vector3 max = lights[lightType][i + a]->boundingBox.getMax();
			
			vertices.push_back(TGen::Vector3(min.x, min.y, max.z));
			vertices.push_back(TGen::Vector3(max.x, min.y, max.z));
			vertices.push_back(TGen::Vector3(max.x, max.y, max.z));
			vertices.push_back(TGen::Vector3(min.x, max.y, max.z));

			vertices.push_back(TGen::Vector3(max.x, min.y, min.z));
			vertices.push_back(TGen::Vector3(min.x, min.y, min.z));
			vertices.push_back(TGen::Vector3(min.x, max.y, min.z));
			vertices.push_back(TGen::Vector3(max.x, max.y, min.z));
			
			vertices.push_back(TGen::Vector3(max.x, min.y, max.z));
			vertices.push_back(TGen::Vector3(max.x, min.y, min.z));
			vertices.push_back(TGen::Vector3(max.x, max.y, min.z));
			vertices.push_back(TGen::Vector3(max.x, max.y, max.z));
			
			vertices.push_back(TGen::Vector3(min.x, min.y, min.z));
			vertices.push_back(TGen::Vector3(min.x, min.y, max.z));
			vertices.push_back(TGen::Vector3(min.x, max.y, max.z));
			vertices.push_back(TGen::Vector3(min.x, max.y, min.z));
			
			vertices.push_back(TGen::Vector3(min.x, min.y, min.z));
			vertices.push_back(TGen::Vector3(max.x, min.y, min.z));
			vertices.push_back(TGen::Vector3(max.x, min.y, max.z));
			vertices.push_back(TGen::Vector3(min.x, min.y, max.z));
			
			
			app.renderer.setLight(a, lights[lightType][i + a]->light);
		}			
		
		vb->bufferData(&vertices[0], sizeof(LightVertexDecl::Type) * vertices.size(), 0);
		//renderFillQuad(lightDirectionalMaterial, TGen::lexical_cast<std::string>(a) + "lights");
		
		TGen::Texture * textures[] = {NULL, colorMap, normalMap, miscMap, depthMap};
	//	app.renderer.setRenderTarget(depthTarget);

		lightPositionalMaterial->render(app.renderer, TGen::Engine::VBRenderable(vb, TGen::PrimitiveQuads, vertices.size()), "1lights", 9, textures, this);
		
	}		
	
	delete vb;*/
	
	// TODO: fixa det här med specialization
	// cacha varje materials specs, cachedSpecializations[getMaterial("directional")][2]  pekar på "2light"
	// entryn byggs upp när materialet först introduceras
	
	//renderFillQuad(lightMaterials[lightType * lightBatchSize + a ]);

	
	// TODO: om man stöter på "pass" i materialblocket (innan lod) så skapas lod 9
	// TODO: specializations/lod ska kunna ha en flagga som säger om den nivån verkligen ska länkas vid link
	//         om den är satt så länkas den inte.   Eller nej, borde finnas en lista som kollas mot när man parsar material
	//			  om en lod finns med i den listan så laddas den inte in
	//         sen kan man ha en annan flagga som berättar om den lod:en verkligen använts någon gång
	//       hur arbetar man med detta: gå runt i en värld och titta runt på alla möjliga sätt, som när man spelar
	//       sen sparar man alla lods flaggor till en fil, de som är false. detta används sen när man laddar in alla material
	//       vissa material ska inte kunna dödas såhär, t ex lampor. men då sätter man en param: noPreCull (i paramblocket)
	// TODO: #loop 0 to r_lightBatchSize (read only)
	
	if (vars.postProcessing) {
		postProcessing(viewport);		
	}

}

// No world loaded, ie, no map:
void TGen::Engine::DeferredRenderer::renderWorldless(scalar dt) {
	app.renderer.setClearColor(TGen::Color::Red);
	app.renderer.clearBuffers(TGen::ColorBuffer);
	
}

// MÅSTE GLLIGHTFV FÖRSt, kordinatsystemet.

void TGen::Engine::DeferredRenderer::postProcessing(const TGen::Rectangle & viewport) {
	// TODO: flökar säkert sönder depthbuffern.. disabla
	// TODO: alphan som clearas med ska justeras beroende på fps
	
	app.renderer.setViewport(downsampleSize);
	app.renderer.setRenderTarget(postTargets2);
	
	/*if (vars.lumTrace) {	// måste ha blendFunc add add för det här...
		app.renderer.setColor(TGen::Color(0.0, 0.0, 0.0, 0.1));
		renderFillQuad(rhwOnlyColorMaterial);
	}*/
	
	renderPostFillQuad(postLuminanceMaterial);
	
	for (int i = 0; i < vars.bloomBlurPasses; ++i) {	// pingpong:a gaussian blur
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

void TGen::Engine::DeferredRenderer::renderFillQuad(TGen::Material * material, const std::string & specialization) {
	if (!material || !screenFillMesh)
		throw TGen::RuntimeException("DeferredRenderer::renderFillQuad", "missing resources");

	TGen::Texture * textures[] = {NULL, colorMap, normalMap, miscMap, depthMap};
	
	material->render(app.renderer, *screenFillMesh, specialization, 9, textures, this);	
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
	if (name == "$texelwidth")		// TODO: helt kontextlöst... kanske inte ska vara så? byta namn alltså, texelwidth -> downsampleTexelWidth
		var = scalar(1.0 / downsampleSize.width);
	else if (name == "$texelheight")
		var = scalar(1.0 / downsampleSize.height);
	else if (name == "$lummin")
		var = vars.lumMin;
	else if (name == "$lummultiplier")
		var = vars.lumMultiplier;
	else if (name == "$lumkilltrace")
		var = !vars.lumTrace;
	else if (name == "$numlights")
		var = lastNumLights;		
	else
		app.logs.warning["dfr"] << "nothing to bind for '" << name << "'!" << TGen::endl;
}

// TODO: fixa simpel fps

int TGen::Engine::DeferredRenderer::ceilPowerOfTwo(int value) {
	int power = 2;
	while (value > power) {
		power *= 2;
	}
	
	return power;
}

void TGen::Engine::DeferredRenderer::setWorld(TGen::Engine::World * newWorld) {
	world = newWorld;
}
