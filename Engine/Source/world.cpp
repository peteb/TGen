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
{
	app.logs.info["wrld+"] << "initializing world..." << TGen::endl;
	mainCam = new TGen::Camera("maincam", TGen::Vector3(0.0f, 0.0f, -1.0f));
	mainCam->setClip(0.1f, 500.0f);
	mainCam->setLod(0.0f, 500.0f);
	mainCam->setOrientation(TGen::Vector3(0.0f, 0.0f, 1.0f));
	
	sceneRoot.addChild(mainCam);
	
	
	TGen::MeshGeometryLinkList meshList;
	sceneRoot.addChild(new TGen::SceneNode("weapon", TGen::Vector3(0.0f, 0.0f, 1.0f)));
	sceneRoot.getChild("weapon")->addFace(TGen::Face(meshList.attach(new TGen::MeshGeometry("models/railgun.md3")), "railgunMaterial"));
	
	meshList.relink(app.globalResources);
	sceneRoot.traverse(TGen::FaceLinker(app.globalResources));
	sceneRoot.traverse(TGen::ScenePrinter(std::cout));
	//

	sceneRoot.update();
}

TGen::Engine::World::~World() {
	delete mainCam;
	app.logs.info["wrld-"] << "shut down" << TGen::endl;
}

TGen::Camera * TGen::Engine::World::getCamera(const std::string & name) {
	return mainCam;
}

TGen::RenderList & TGen::Engine::World::getRenderList(TGen::Camera * camera) {
	renderList.clear();
	sceneRoot.traverse(TGen::RenderFiller(renderList, *camera));
	return renderList;
}

void TGen::Engine::World::update(scalar dt) {
	//sceneRoot.getChild("weapon")->setPosition(sceneRoot.getChild("weapon")->getLocalPosition() + TGen::Vector3(dt, 0.0f, 0.0f));
	TGen::Matrix4x4 rot = TGen::Matrix4x4::RotationY(TGen::Radian(dt * 0.2));
	
	sceneRoot.getChild("weapon")->setOrientation(rot * TGen::Vector3(sceneRoot.getChild("weapon")->getLocalOrientation()));
	sceneRoot.update();
}

