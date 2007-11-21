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

TGen::Engine::World::World(TGen::Engine::App & app)
	: app(app)
	, sceneRoot("root")
	, mainCam(NULL)
	, lightList(100)
{
	app.logs.info["wrld+"] << "initializing world..." << TGen::endl;
	mainCam = new TGen::Camera("maincam", TGen::Vector3(0.0f, 1.0f, -0.5f));
	mainCam->setClip(0.1f, 500.0f);
	mainCam->setLod(0.0f, 500.0f);
	mainCam->setOrientation(TGen::Vector3(0.0f, 0.4f, 1.0f).normalize());
	
	sceneRoot.addChild(mainCam);
	
	
	TGen::MeshGeometryLinkList meshList;
	sceneRoot.addChild(new TGen::SceneNode("weapon", TGen::Vector3(0.0f, 0.0f, 1.0f)));
	sceneRoot.getChild("weapon")->addFace(TGen::Face(meshList.attach(new TGen::MeshGeometry("models/railgun.md3")), "railgunMaterial"));

	// TODO: scenegraphen buggar!!!!!!! hårt!!!!!
	
	meshList.relink(app.globalResources);
	sceneRoot.traverse(TGen::FaceLinker(app.globalResources));
	sceneRoot.traverse(TGen::ScenePrinter(std::cout));
	//
	// börja jobba på portalbanorna
	
	sceneRoot.update();
}

TGen::Engine::World::~World() {
	delete mainCam;
	app.logs.info["wrld-"] << "shut down" << TGen::endl;
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
	
	sceneRoot.traverse(TGen::RenderFiller(renderList, *camera));	
	
	/*TGen::Light light;
	light.position = TGen::Vector4(0.0f, 1.0f, 0.0f, 0.0f);		// directional
	light.diffuse = TGen::Color(0.3, 0.0, 0.0, 1.0);
	light.specular = TGen::Color(1.0, 1.0, 1.0, 1.0);
	
	lightList.addLight(light);

	light.position = TGen::Vector4(0.0f, -1.0f, 0.0f, 0.0f);		// directional
	light.diffuse = TGen::Color(0.0, 0.0, 0.6, 1.0);
	light.specular = TGen::Color(1.0, 1.0, 1.0, 1.0);
	
	//for (int i = 0; i < 31; ++i)
		lightList.addLight(light);
	*/
	
	scalar step = (2.0 * TGen::PI) / 8.0;
	
	/*for (int i = 0; i < 8; ++i) {
		TGen::Light light;
		light.position = TGen::Vector4(0.0f, TGen::Cos(TGen::Radian(step * scalar(i))), TGen::Sin(TGen::Radian(step * scalar(i))), 0.0);		
		light.diffuse = TGen::Color(0.0, 0.3, 0.0, 1.0);
		light.specular = TGen::Color((8.0 - scalar(i)) / 8.0, scalar(i) / 8.0, (scalar(i)) / 8.0, 1.0);
		lightList.addLight(light);
	}*/
	
	TGen::Light light;
	light.position = TGen::Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	light.diffuse = TGen::Color(0.1, 0.1, 0.0, 1.0);
	light.specular = TGen::Color(1.0, 1.0, 0.0, 1.0);
	
	lightList.addLight(light);

	light.position = TGen::Vector4(0.0f, 0.0f, -1.0f, 0.0f);
	light.diffuse = TGen::Color(0.0, 0.1, 0.0, 1.0);
	light.specular = TGen::Color(1.0, 1.0, 1.0, 0.5);
	
	lightList.addLight(light);
	
	light.position = TGen::Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	light.diffuse = TGen::Color(0.0, 0.0, 0.1, 1.0);
	light.specular = TGen::Color(0.0, 0.0, 1.3, 1.0);
	
	lightList.addLight(light);
	
	light.position = TGen::Vector4(0.0f, -1.0f, 0.0f, 0.0f);
	light.diffuse = TGen::Color(0.0, 0.1, 0.1, 1.0);
	light.specular = TGen::Color(0.0, 0.3, 0.3, 1.0);
	
	lightList.addLight(light);
	// TODO: light ska inte bestämma specularity
}

void TGen::Engine::World::update(scalar dt) {
	//sceneRoot.getChild("weapon")->setPosition(sceneRoot.getChild("weapon")->getLocalPosition() + TGen::Vector3(dt, 0.0f, 0.0f));
	TGen::Matrix4x4 rot = TGen::Matrix4x4::RotationY(TGen::Radian(dt * 0.2));
	
	sceneRoot.getChild("weapon")->setOrientation(rot * TGen::Vector3(sceneRoot.getChild("weapon")->getLocalOrientation()));
	sceneRoot.update();
}

TGen::Color TGen::Engine::World::getAmbientLight() {
	return TGen::Color(1.0, 1.0, 1.0, 1.0);
}

// rendreraren ber world att uppdatera sig internt för en viss kamera, sen frågar den efter renderlist, lights, osv
