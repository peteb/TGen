/*
 *  world.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/16/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "world.h"
#include "light.h"
#include "entity.h"
#include "filesystem.h"
#include "file.h"
#include "scene/node.h"
#include "log.h"

TGen::Engine::World::World(TGen::Engine::Filesystem & filesystem, TGen::Engine::ResourceManager & resources, TGen::Engine::StandardLogs & logs, TGen::VertexDataSource & dataSource, const std::string & mapname)
	: filesystem(filesystem)
	, logs(logs)
	, sceneSubsystem(resources, filesystem, logs, dataSource)
	, physicsSubsystem(logs)
	, sceneRoot("root")
	, mainCam(NULL)
	, lightList(100)
	, entityFactory(logs)
{
	logs.info["world+"] << "initializing world '" << mapname << "'..." << TGen::endl;
	
	entityFactory.registerSubsystem("sceneNode", &sceneSubsystem);
	entityFactory.registerSubsystem("sceneCamera", &sceneSubsystem);
	entityFactory.registerSubsystem("sceneLight", &sceneSubsystem);
	entityFactory.registerSubsystem("sceneMap", &sceneSubsystem);

	entityFactory.registerSubsystem("physBody", &physicsSubsystem);
	entityFactory.registerSubsystem("physGeom", &physicsSubsystem);
	entityFactory.registerSubsystem("physJoint", &physicsSubsystem);
	
	// TODO: kontrollera om entiteter ska map cullas, vanligtvis på. stäng av med noMapCull eller nått
	// TODO: entiteter ska kunna höra till banan och på så vis bara bli rendrerade om de är i ett rum som syns
	//       kan kanske fixas genom att överlagra funktionen som ger faces åt renderlist att rendrera, att den då går igenom de rum som syns, 
	//       men alla entiteter i banan ska uppdateras när banan uppdateras.
	// TODO: överlagra addChild för map, den för in entiteten i den area som origin pekar på
	// TODO: sen i fysikmotorn borde man kunna låsa de objekt som inte är i något aktuellt rum, slippa uppdatera en massa. borde dock följa med hierarkiskt.
	// TODO: riktig file-logger. som resettar filen när man startar
	// TODO: kunna pruna ett materials resurser, men om de används på andra ställen då? då måste refcount in i bilden...


	TGen::Engine::File * entitiesFile = filesystem.openRead("/maps/" + mapname + "/entities");

	
	TGen::PropertyTreeParser propParser;
	TGen::PropertyTree props = propParser.parse(entitiesFile->readAll().c_str());
	delete entitiesFile;
	
	for (int i = 0; i < props.getNumNodes(); ++i) {
		TGen::Engine::Entity * entity = entityFactory.createEntity(props.getNode(i));
		
		entities.addEntity(entity);
		/*if (entities.find(entity->getName()) == entities.end()) {
			entities.insert(EntityMap::value_type(entity->getName(), entity));
		}
		else {
			app.logs.warning["world"] << "entity '" << entity->getName() << "' already set!" << TGen::endl;
			delete entity;
		}*/
	}
	
	entities.linkGlobally();
	
	//mainCam = dynamic_cast<TGen::Camera *>(sceneSubsystem.getComponent("maincam")->getSceneNode());
	//if (!mainCam)
	//	throw TGen::RuntimeException("World::World", "maincam not defined");

	sceneSubsystem.link();
	sceneSubsystem.getSceneRoot().update();

	
	/*sceneRoot.addChild(mainCam);
	
	
	TGen::MeshGeometryLinkList meshList;
	sceneRoot.addChild(new TGen::SceneNode("weapon", TGen::Vector3(0.0f, 0.0f, 1.0f)));
	sceneRoot.getChild("weapon")->addFace(TGen::Face(meshList.attach(new TGen::MeshGeometry("models/railgun.md3")), "railgunMaterial"));	// hur anger md3:an material?




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
	logs.info["world-"] << "shutting down..." << TGen::endl;

	/*for (EntityMap::iterator iter = entities.begin(); iter != entities.end(); ++iter) {
		delete iter->second;
	}*/
	
}

TGen::Camera * TGen::Engine::World::getCamera(const std::string & name) {
	return static_cast<TGen::Camera *>(sceneSubsystem.getSceneRoot().getChild(name));
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
	
	for (int i = 0; i < renderList.getNumUserInfo(); ++i) {
		TGen::RenderList::UserInfo & userInfo = renderList.getUserInfo(i);
		if (userInfo.value == TGen::Engine::UserTypeLight) {
			lightList.addLight(static_cast<TGen::Engine::Light *>(userInfo.data));
		}
	}
			
	// TODO: light ska inte bestämma specularity
}
// TODO: RenderFiller ska fråga varje scene node efter geoms, sen kan PortalNode överlagra den och fräsaaaaa
// men då ska den metoden helst ta en kamera som parameter

void TGen::Engine::World::update(scalar dt) {
	//sceneRoot.getChild("weapon")->setPosition(sceneRoot.getChild("weapon")->getLocalPosition() + TGen::Vector3(dt, 0.0f, 0.0f));
	TGen::Matrix4x4 rot = TGen::Matrix4x4::RotationX(TGen::Radian(dt * 0.1));
	
	//sceneSubsystem.getSceneRoot().getChild("test5")->setOrientation(rot * TGen::Vector3(sceneSubsystem.getSceneRoot().getChild("test5")->getLocalOrientation()));
	//sceneSubsystem.getSceneRoot().getChild("testmap")->setOrientation(rot * TGen::Vector3(sceneSubsystem.getSceneRoot().getChild("testmap")->getLocalOrientation()));
	// TODO: add lights IN the scene node of the map
	
	sceneSubsystem.getSceneRoot().update();	
	physicsSubsystem.update(dt);
}

TGen::Color TGen::Engine::World::getAmbientLight() {
	return TGen::Color(0.1, 0.1, 0.1, 1.0);
}

TGen::Engine::Player * TGen::Engine::World::createPlayer() {
	TGen::Camera * camera = new TGen::Camera("maincam", TGen::Vector3(0.0f, 2.0f, 0.0f), TGen::Rotation::Identity);
	sceneSubsystem.getSceneRoot().addChild(camera);
	
	// TODO: ange inte playerstuff i entities, men player ska vara en TGen::Engine::Entity!
	//       dvs, populera entiteten med components här typ.
	
	return NULL;
}
// rendreraren ber world att uppdatera sig internt för en viss kamera, sen frågar den efter renderlist, lights, osv
