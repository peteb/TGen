/*
 *  deferredscenerenderer.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 10/31/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "log.h"
#include "renderer/deferred/renderer.h"
#include "filesystem.h"
#include "file.h"
#include "variableregister.h"
#include "world.h"
#include "light.h"
#include "vbrenderable.h"
#include "metacreator.h"
#include "resourcemanager.h"
#include "variableregister.h"
#include <tgen_graphics.h>
#include "playercontroller.h"

TGen::Engine::DeferredRenderer::DeferredRenderer(TGen::Renderer & renderer, 
																 TGen::Engine::StandardLogs & logs, 
																 TGen::Engine::VariableRegister & variables, 
																 TGen::Engine::ResourceManager & resources) 
	: TGen::Engine::WorldRenderer(renderer)
	, logs(logs)
	, variables(variables)
	, resources(resources)
	, vars(variables)
	, mainCamera(NULL)
	, lastNumLights(0)
	, lightBatchSize(8)
	, lightMaterials(NULL)
	//, world(NULL)
	, metaLines(renderer, 1000000, TGen::PrimitiveLines, TGen::UsageStream)
{
	logs.info["dfr+"] << "deferred renderer initializing..." << TGen::endl;
	
	rhwNoTransformShader = resources.getShaderProgram("rhwNoTransform");
	rhwOnlyColorMaterial = resources.getMaterial("deferred/rhwOnlyColor");
	screenFillMesh = resources.getMesh("gen:fillquad");
	lightAmbientMaterial = resources.getMaterial("deferred/lightAmbient");
	lightDirectionalMaterial = resources.getMaterial("deferred/lightDirectional");
	lightPositionalMaterial = resources.getMaterial("deferred/lightPositional");
	postLuminanceMaterial = resources.getMaterial("post/luminance");
	postGaussianHorizMaterial = resources.getMaterial("post/gaussianHoriz");
	postGaussianVertMaterial = resources.getMaterial("post/gaussianVert");
	postFinalBloom = resources.getMaterial("post/finalBloom");
	metaNormalMaterial = resources.getMaterial("meta/normal");
	
	TGen::Rectangle mapSize(int(this->variables["env_width"]), int(this->variables["env_height"]));
	
	if (vars.forceBinaryMRT)
		mapSize = TGen::Rectangle(ceilPowerOfTwo(mapSize.width), ceilPowerOfTwo(mapSize.height));
	
	try {
		colorMap = depthMap = normalMap = miscMap = postMap1 = postMap2 = postMap3 = NULL;
		mapTargets = postTargets1 = postTargets2 = postTargets3 = NULL;
		
		createResources(mapSize);
	} // TODO: clean up, auto_ptr
	catch (const TGen::RuntimeException & e) {	// trying power-of-two texture size
		delete colorMap; delete depthMap; delete normalMap; delete miscMap; delete mapTargets; delete postMap1; delete postMap2; delete postTargets1; delete postTargets2; delete postMap3; delete postTargets3;
		colorMap = depthMap = normalMap = miscMap = postMap1 = postMap2 = postMap3 = NULL;
		mapTargets = postTargets1 = postTargets2 = postTargets3 = NULL;
		
		mapSize = TGen::Rectangle(ceilPowerOfTwo(mapSize.width), ceilPowerOfTwo(mapSize.height));

		logs.warning["dfr+"] << e << TGen::endl;
		logs.warning["dfr+"] << "trying " << std::string(mapSize) << "..." << TGen::endl;
		
		createResources(mapSize);
	}
	
	logs.info["dfr+"] << "mrts size: " << std::string(mapSize) << TGen::endl;
	
	
	lightMaterials = new TGen::Material*[lightBatchSize * 3];
	//loadLightMaterial("deferred/lightDirectional", 0);
	
	logs.info["dfr+"] << "light batch size: " << lightBatchSize << TGen::endl;
	
	TGen::Rectangle viewport = renderer.getViewport();
	renderer.setRenderTarget(postTargets2);
	renderer.setViewport(downsampleSize);
	renderer.clearBuffers(TGen::ColorBuffer);

	renderer.setRenderTarget(NULL);
	renderer.setViewport(viewport);
	
	logs.info["dfr+"] << "bloom downsampling: " << std::string(downsampleSize) << TGen::endl;
	logs.info["dfr+"] << "initialized" << TGen::endl;
}

TGen::Engine::DeferredRenderer::~DeferredRenderer() {
	logs.info["dfr-"] << "shutting down..." << TGen::endl;

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

void TGen::Engine::DeferredRenderer::loadLightMaterial(const std::string & name, int materialId, TGen::Engine::ResourceManager & resources) {
	for (int i = 0; i < lightBatchSize; ++i)
		lightMaterials[materialId * lightBatchSize + i] = resources.getMaterial(name + TGen::lexical_cast<std::string>(i));
}

void TGen::Engine::DeferredRenderer::createResources(const TGen::Rectangle & mapSize) {
	downsampleSize = mapSize / scalar(pow(2.0f, vars.bloomDownsampling));
	
	colorMap = renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	normalMap = renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	miscMap = renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	depthMap = renderer.createTexture(mapSize, TGen::DEPTH24, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps /*| TGen::TextureRectangle*/);	// TODO: ubyte på depth? wtf?
	
	postMap1 = renderer.createTexture(mapSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	postMap2 = renderer.createTexture(downsampleSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	postMap3 = renderer.createTexture(downsampleSize, TGen::RGBA, TGen::TypeUnsignedByte, TGen::TextureNoMipmaps);
	
	postMap1->setWrapMode(TGen::TextureWrapClamp, TGen::TextureWrapClamp);
	postMap2->setWrapMode(TGen::TextureWrapClamp, TGen::TextureWrapClamp);
	postMap3->setWrapMode(TGen::TextureWrapClamp, TGen::TextureWrapClamp);

	/*colorMap->setFilterMode(TGen::TextureFilterNearest, TGen::TextureFilterNearest);
	normalMap->setFilterMode(TGen::TextureFilterNearest, TGen::TextureFilterNearest);
	depthMap->setFilterMode(TGen::TextureFilterNearest, TGen::TextureFilterNearest);
	 
	 testa rectangle på depth igen.... nej, måste ju vara samma format på alla attachments. men iofs, depth != color
	*/
	
	renderTarget = renderer.createFrameBuffer();
	
	mapTargets = renderer.createFrameBuffer();
	mapTargets->attachColor(colorMap);
	mapTargets->attachColor(normalMap);
	mapTargets->attachColor(miscMap);
	mapTargets->attachDepth(depthMap);
	
	postTargets1 = renderer.createFrameBuffer();
	postTargets1->attachColor(postMap1);
	//postTargets1->attachDepth(depthMap);
	
	postTargets2 = renderer.createFrameBuffer();
	postTargets2->attachColor(postMap2);
	
	postTargets3 = renderer.createFrameBuffer();
	postTargets3->attachColor(postMap3);
	
	//depthTarget = app.renderer.createFrameBuffer();
	//depthTarget->attachDepth(depthMap);
	
	// TODO: USE ONLY ONE FBO!!!!!!!!! swap bound textures
	
	mrtSize = mapSize;
}

void TGen::Engine::DeferredRenderer::renderWorld(TGen::Engine::World & world, TGen::Camera * camera, scalar dt) {
	/*if (!world) {
		renderWorldless(dt);
		return;
	}
	*/
	
	mainCamera = camera; //world.getCamera("maincam");   // TODO: mainCamera?!

	
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
	
	world.prepareLists(mainCamera);
	TGen::RenderList & renderList = world.getRenderList();
	TGen::Engine::LightList & lightList = world.getLightList();
	
	renderList.sort(*mainCamera, "default");
	
	//std::cout << "renderlist: " << renderList.getNumFaces() << std::endl;
	
	TGen::Engine::MetaCreator mc;
	
	metaLines.beginBatch();
	mc.writeAxes(TGen::Matrix4x4::Identity, metaLines);
	//renderList.writeMeta(TGen::MetaNormals, TGen::Matrix4x4::Identity, metaLines);
	//renderList.writeMeta(TGen::MetaPortals, TGen::Matrix4x4::Identity, metaLines);
	
	metaLines.endBatch();
	
	TGen::Rectangle viewport = renderer.getViewport();
	
	
	// UPDATE MAPS (color, normal, spec, depth, etc)
	
	renderer.setViewport(mrtSize);
	renderer.setClearColor(TGen::Color::Black);

	
	renderTarget->reset();
	renderTarget->setColorUnit(0, colorMap);
	renderTarget->setColorUnit(1, normalMap);
	renderTarget->setColorUnit(2, miscMap);
	renderTarget->setDepthUnit(0, depthMap);
	renderer.setRenderTarget(renderTarget);

	
	
	
	
	
	
	

	renderer.clearBuffers(TGen::ColorBuffer | TGen::DepthBuffer);
	renderer.setAmbientLight(world.getAmbientLight());

	renderer.setTransform(TGen::TransformProjection, mainCamera->getProjection());
	renderList.render(renderer, mainCamera->getTransform(), mainCamera->getLod(), "default");

	renderer.setTransform(TGen::TransformWorldView, mainCamera->getTransform());
	metaNormalMaterial->render(renderer, metaLines, "default", 9, NULL, NULL);
	

	// TODO: var ska det här vara egentligen....
	
	//vars.postProcessing = false;
	// postprocessing kostar 110 fps
	
	/*if (vars.postProcessing) {
	 renderer.setRenderTarget(postTargets1);
	 renderer.setViewport(mrtSize);
	 }
	 else {
	 renderer.setRenderTarget(NULL);
	 renderer.setViewport(viewport);
	 }*/	
	
	//app.renderer.clearBuffers(TGen::DepthBuffer);

	// AMBIENT TO RESULT
	//renderFillQuad(lightAmbientMaterial);
	
	// LIGHTS TO RESULT
	TGen::Engine::LightList::LightMap & lightsByMaterial = lightList.getLightsByMaterial();
	for (TGen::Engine::LightList::LightMap::iterator iter = lightsByMaterial.begin(); iter != lightsByMaterial.end(); ++iter) {
		//std::cout << "MATERIAL " << iter->first << std::endl;
		
		// TODO: OM EN LAMPA HAR FACES SÅ RENDRERA DEM ist för fillquad!!!! det är bounding boxes
		//       om man är innanför en bbox för ett ljus rita backfaces, annars bara frontfaces
		//       för mindre ljus kan man köra bbox. kolla mot bounding-sphere eller aabb.
		
		
		TGen::Texture * textures[] = {NULL, colorMap, normalMap, miscMap, depthMap};
		
		
		renderer.setTransform(TGen::TransformProjection, mainCamera->getProjection());		// borde inte vara nödvändigt...
	
		
		TGen::Engine::LightList::LightArray * lights = iter->second;
		if (lights) {
			lightBatchSize = 1;
			for (int i = 0; i < lights->size(); i += lightBatchSize) {
				int a = 0;
				
				for (; a < lightBatchSize && i + a < lights->size(); ++a) {
					renderer.setTransform(TGen::TransformWorldView, mainCamera->getTransform() * (*lights)[a + i]->getTransform());
					
					if ((*lights)[a + i]->getType() == TGen::Engine::LightDirectional)
						(*lights)[a + i]->getLightProperties().position = TGen::Vector4((*lights)[a + i]->getTransform().getZ().normalize(), 0.0f);						
					else
						(*lights)[a + i]->getLightProperties().position = TGen::Vector4(0.0f, 0.0f, 0.0f, 1.0f); //(*lights)[a]->getWorldPosition();
					
					renderer.setLight(a, (*lights)[a + i]->getLightProperties());

					if ((*lights)[a + i]->getType() == TGen::Engine::LightDirectional)
						renderFillQuad(iter->first, TGen::lexical_cast<std::string>(a + 1 + i) + "lights");	// TODO: optimize						
					else {
					//	glIsTexture(1);
						(*lights)[a + i]->getMaterial()->render(renderer, TGen::SceneNodeRenderable(*(*lights)[a + i]), TGen::lexical_cast<std::string>(a + 1 + i) + "lights", 9, textures, this);
					}
						
						//lightPositionalMaterial->render(renderer, TGen::SceneNodeRenderable(*(*lights)[a]), TGen::lexical_cast<std::string>(a + 1) + "lights", 9, NULL, NULL);
						
					
				}
				
				//lightPositionalMaterial->render(renderer, *model, TGen::lexical_cast<std::string>(a) + "lights", 9, NULL, NULL);

				
				
			}
		}
	}
	
	// TODO: bbox för lights... dvs genCube
	
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
	renderer.setClearColor(TGen::Color::Red);
	renderer.clearBuffers(TGen::ColorBuffer);
	
}

// MÅSTE GLLIGHTFV FÖRSt, kordinatsystemet.

void TGen::Engine::DeferredRenderer::postProcessing(const TGen::Rectangle & viewport) {
	// TODO: flökar säkert sönder depthbuffern.. disabla
	// TODO: alphan som clearas med ska justeras beroende på fps
	
	renderer.setViewport(downsampleSize);
	renderer.setRenderTarget(postTargets2);
	
	/*if (vars.lumTrace) {	// måste ha blendFunc add add för det här...
		renderer.setColor(TGen::Color(0.0, 0.0, 0.0, 0.1));
		renderFillQuad(rhwOnlyColorMaterial);
	}*/
	
	renderPostFillQuad(postLuminanceMaterial);
	
	for (int i = 0; i < vars.bloomBlurPasses; ++i) {	// pingpong:a gaussian blur
		renderer.setRenderTarget(postTargets3);	
		renderPost2FillQuad(postGaussianHorizMaterial);
	
		renderer.setRenderTarget(postTargets2);
		renderPost3FillQuad(postGaussianVertMaterial);
	}
	
	renderer.setRenderTarget(NULL);
	renderer.setViewport(viewport);
	
	renderPostFinalQuad(postFinalBloom);
}

void TGen::Engine::DeferredRenderer::renderFillQuad(TGen::Material * material) {
	if (!material || !screenFillMesh)
		throw TGen::RuntimeException("DeferredRenderer::renderFillQuad", "missing resources");

	TGen::Texture * textures[] = {NULL, colorMap, normalMap, miscMap, depthMap};
	
	material->render(renderer, *screenFillMesh, "default", 9, textures, this);
}

void TGen::Engine::DeferredRenderer::renderFillQuad(TGen::Material * material, const std::string & specialization) {
	if (!material || !screenFillMesh)
		throw TGen::RuntimeException("DeferredRenderer::renderFillQuad", "missing resources");

	TGen::Texture * textures[] = {NULL, colorMap, normalMap, miscMap, depthMap};
	
	material->render(renderer, *screenFillMesh, specialization, 9, textures, this);	
}

void TGen::Engine::DeferredRenderer::renderPostFillQuad(TGen::Material * material) {
	TGen::Texture * textures[] = {NULL, colorMap, normalMap, miscMap, depthMap};
	
	material->render(renderer, *screenFillMesh, "default", 9, textures, this);	
}

void TGen::Engine::DeferredRenderer::renderPost2FillQuad(TGen::Material * material) {
	TGen::Texture * textures[] = {NULL, postMap2, normalMap, miscMap, depthMap};
	
	material->render(renderer, *screenFillMesh, "default", 9, textures, this);	
}

void TGen::Engine::DeferredRenderer::renderPost3FillQuad(TGen::Material * material) {
	TGen::Texture * textures[] = {NULL, postMap3, normalMap, miscMap, depthMap};
	
	material->render(renderer, *screenFillMesh, "default", 9, textures, this);	
}

void TGen::Engine::DeferredRenderer::renderPostFinalQuad(TGen::Material * material) {
	TGen::Texture * textures[] = {NULL, colorMap, postMap2, miscMap, depthMap};
	
	material->render(renderer, *screenFillMesh, "default", 9, textures, this);		
}

void TGen::Engine::DeferredRenderer::updateShaderVariable(TGen::ShaderVariable & var, const std::string & name) {
	if (name == "$texelwidth")		// TODO: helt kontextlöst... kanske inte ska vara så? byta namn alltså, texelwidth -> downsampleTexelWidth
		var = float(1.0 / downsampleSize.width);
	else if (name == "$texelheight")
		var = float(1.0 / downsampleSize.height);
	else if (name == "$lummin")
		var = vars.lumMin;
	else if (name == "$lummultiplier")
		var = vars.lumMultiplier;
	else if (name == "$lumkilltrace")
		var = !vars.lumTrace;
	else if (name == "$numlights")
		var = lastNumLights;		
	else if (name == "$rtsize")
		var = TGen::Vector2(mrtSize.width, mrtSize.height);
	else if (name == "$eye" && mainCamera)
		var = mainCamera->getTransform().getOrigin();
	else
		throw TGen::RuntimeException("DeferredRenderer::updateShaderVariable", "nothing to bind for '" + name + "'!");
		
		//logs.warning["dfr"] << "nothing to bind for '" << name << "'!" << TGen::endl;
}

// TODO: fixa simpel fps

int TGen::Engine::DeferredRenderer::ceilPowerOfTwo(int value) {
	int power = 2;
	while (value > power) {
		power *= 2;
	}
	
	return power;
}

/*void TGen::Engine::DeferredRenderer::setWorld(TGen::Engine::World * newWorld) {
	world = newWorld;
}
*/
