/*
 *  world.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/16/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "world.h"
#include "app.h"
#include "light.h"
#include "entity.h"

TGen::Engine::World::World(TGen::Engine::App & app)
	: app(app)
	, sceneSubsystem(*this)
	, sceneRoot("root")
	, mainCam(NULL)
	, lightList(100)
	, entityFactory(app.logs)
{
	app.logs.info["world+"] << "initializing world..." << TGen::endl;
	
	entityFactory.registerSubsystem("sceneNode", &sceneSubsystem);
	entityFactory.registerSubsystem("sceneCamera", &sceneSubsystem);
	entityFactory.registerSubsystem("sceneLight", &sceneSubsystem);
	
	// TODO: world("mapname")   
	// TODO: riktig file-logger. som resettar filen när man startar
	char * propert =
		"test {\n"
			"sceneNode {\n"
				"model \"models/railgun.md3\"\n"
				"origin \"0 0 1\"\n"
				"material \"railgunMaterial\"\n"
			"}\n"
		"}\n"
	
		"maincam {\n"
			"sceneCamera {\n"
				"origin \"0 1 -0.5\"\n"
				"range \"500\"\n"
				"orientation \"0 0.4 1.0\"\n"
			"}\n"
		"}\n"
		;
	
	TGen::PropertyTreeParser propParser;
	TGen::PropertyTree props = propParser.parse(propert);
	
	for (int i = 0; i < props.getNumNodes(); ++i) {
		TGen::Engine::Entity * entity = entityFactory.createEntity(props.getNode(i));
		
		if (entities.find(entity->getName()) == entities.end()) {
			entities.insert(EntityMap::value_type(entity->getName(), entity));
		}
		else {
			app.logs.warning["world"] << "entity '" << entity->getName() << "' already set!" << TGen::endl;
			delete entity;
			entity = NULL;
		}
	}
	
	//exit(1);
	
	mainCam = new TGen::Camera("maincam", TGen::Vector3(0.0f, 1.0f, -0.5f));
	mainCam->setClip(0.1f, 500.0f);
	mainCam->setLod(0.0f, 500.0f);
	mainCam->setOrientation(TGen::Vector3(0.0f, 0.4f, 1.0f).normalize());

	sceneSubsystem.getSceneRoot().addChild(mainCam);
	sceneSubsystem.link();
	sceneSubsystem.getSceneRoot().update();

	/*sceneRoot.addChild(mainCam);
	
	
	TGen::MeshGeometryLinkList meshList;
	sceneRoot.addChild(new TGen::SceneNode("weapon", TGen::Vector3(0.0f, 0.0f, 1.0f)));
	sceneRoot.getChild("weapon")->addFace(TGen::Face(meshList.attach(new TGen::MeshGeometry("models/railgun.md3")), "railgunMaterial"));	// hur anger md3:an material?

	// TODO: scenegraphen buggar!!!!
	
	meshList.relink(app.globalResources);
	sceneRoot.traverse(TGen::FaceLinker(app.globalResources));
	sceneRoot.traverse(TGen::ScenePrinter(std::cout));
	//
	// börja jobba på portalbanorna
	
	sceneRoot.update();




	TGen::Engine::Light * newLight = new TGen::Engine::Light;
	lights[0] = newLight;
	newLight->type = TGen::Engine::LightDirectional;
	newLight->clipBoundingBox = false;
	newLight->light.position = TGen::Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	newLight->light.diffuse = TGen::Color(0.3, 0.3, 0.3, 1.0);
	newLight->light.specular = TGen::Color(1.0, 1.0, 1.0, 1.0);
	
	newLight = new TGen::Engine::Light;
	lights[1] = newLight;
	newLight->type = TGen::Engine::LightPositional;
	newLight->boundingBox = TGen::AABB(TGen::Vector3(-0.4f, -0.4f, -0.4f), TGen::Vector3(0.4f, 0.4f, 0.4f));
	newLight->light.position = TGen::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	*/
}

// TODO: depthcheck på light volume

TGen::Engine::World::~World() {
	for (EntityMap::iterator iter = entities.begin(); iter != entities.end(); ++iter) {
		delete iter->second;
	}
	
	app.logs.info["world-"] << "shut down" << TGen::endl;
}

TGen::Camera * TGen::Engine::World::getCamera(const std::string & name) {
	return mainCam;
}

TGen::RenderList & TGen::Engine::World::getRenderList() {
	return renderList;
}

TGen::Engine::LightList & TGen::Engine::World::getLightList() {
	return lightList;
}

void TGen::Engine::World::prepareLists(TGen::Camera * camera) {
	renderList.clear();
	lightList.clear();
	
	sceneSubsystem.getSceneRoot().traverse(TGen::RenderFiller(renderList, *camera));		
	
	//lightList.addLight(lights[0]);
	//lightList.addLight(lights[1]);
	
	// TODO: light ska inte bestämma specularity
}

void TGen::Engine::World::update(scalar dt) {
	//sceneRoot.getChild("weapon")->setPosition(sceneRoot.getChild("weapon")->getLocalPosition() + TGen::Vector3(dt, 0.0f, 0.0f));
	TGen::Matrix4x4 rot = TGen::Matrix4x4::RotationY(TGen::Radian(dt * 0.3));
	
	//sceneRoot.getChild("weapon")->setOrientation(rot * TGen::Vector3(sceneRoot.getChild("weapon")->getLocalOrientation()));
	//sceneRoot.update();
}

TGen::Color TGen::Engine::World::getAmbientLight() {
	return TGen::Color(1.0, 1.0, 1.0, 1.0);
}

// rendreraren ber world att uppdatera sig internt för en viss kamera, sen frågar den efter renderlist, lights, osv
