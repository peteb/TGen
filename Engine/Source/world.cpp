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
#include "playercontroller.h"

TGen::Engine::World::World(TGen::Engine::Filesystem & filesystem, TGen::Engine::ResourceManager & resources, TGen::Engine::StandardLogs & logs, TGen::VertexDataSource & dataSource, const std::string & mapname)
	: filesystem(filesystem)
	, logs(logs)
	, sceneSubsystem(resources, filesystem, logs, dataSource)
	, physicsSubsystem(logs, filesystem)
	, soundSubsystem(logs, filesystem)
	, scriptSubsystem(logs)
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
	entityFactory.registerSubsystem("sceneTransform", &sceneSubsystem);
	entityFactory.registerSubsystem("sceneEqNode", &sceneSubsystem);
	
	entityFactory.registerSubsystem("physBody", &physicsSubsystem);
	entityFactory.registerSubsystem("physGeom", &physicsSubsystem);
	entityFactory.registerSubsystem("physJoint", &physicsSubsystem);
	
	entityFactory.registerSubsystem("controller", &controllerSubsystem);
	
	entityFactory.registerSubsystem("soundLocal", &soundSubsystem);
	entityFactory.registerSubsystem("soundGlobal", &soundSubsystem);
	
	entityFactory.registerSubsystem("inventory", &inventorySubsystem);
	entityFactory.registerSubsystem("weapon", &inventorySubsystem);
	
	entityFactory.registerSubsystem("event", &scriptSubsystem);

	// TODO: this class is a hog
	// TODO: sen i fysikmotorn borde man kunna låsa de objekt som inte är i något aktuellt rum, slippa uppdatera en massa. borde dock följa med hierarkiskt.
	// TODO: kunna pruna ett materials resurser, men om de används på andra ställen då? då måste refcount in i bilden...


	loadEntities("/maps/defs");
	loadEntities("/maps/" + mapname + "/entities");
	
	entities.linkGlobally();
	
	sceneSubsystem.link();
	sceneSubsystem.update(0.0f);
	
	soundSubsystem.link();
}

void TGen::Engine::World::loadEntities(const std::string & filename) {
	TGen::Engine::File * entitiesFile = filesystem.openRead(filename);
	
	TGen::PropertyTreeParser propParser;
	TGen::PropertyTree props = propParser.parse(entitiesFile->readAll().c_str());
	delete entitiesFile;
	
	for (int i = 0; i < props.getNumNodes(); ++i) {
		TGen::PropertyTree & node = props.getNode(i);
		
		if (node.getName() == "class") {
			node.setName(node.getAttribute(0));   // change the name of the class to the first attribute
			entityFactory.addClassEntity(node);
		}
		else if (node.getName() == "prototype") {
			node.setName(node.getAttribute(0));
			
			TGen::Engine::EntityRecipe * recipe = entityFactory.createPrototypeEntity(node);
			entities.addPrototype(recipe);
		}
		else {
			TGen::Engine::Entity * entity = entityFactory.createEntity(node);
			
			entities.addEntity(entity);
		}
	}	
}

// TODO: depthcheck på light volume

TGen::Engine::World::~World() {
	logs.info["world-"] << "shutting down..." << TGen::endl;

	/*for (EntityMap::iterator iter = entities.begin(); iter != entities.end(); ++iter) {
		delete iter->second;
	}*/
	
}

TGen::Camera * TGen::Engine::World::getCamera(const std::string & name) {
	throw TGen::RuntimeException("World::getCamera", "shiet, remove this");
	return NULL; //static_cast<TGen::Camera *>(sceneSubsystem.getSceneRoot().getChild(name));
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
			
	
	//TGen::Vector3 pos = controllerSubsystem.getController("player_start")->getPos();
	//std::cout << std::string(pos) << std::endl;
	

	// TODO: light ska inte bestämma specularity
}
// TODO: RenderFiller ska fråga varje scene node efter geoms, sen kan PortalNode överlagra den och fräsaaaaa
// men då ska den metoden helst ta en kamera som parameter

void TGen::Engine::World::update(scalar dt) {
	//sceneRoot.getChild("weapon")->setPosition(sceneRoot.getChild("weapon")->getLocalPosition() + TGen::Vector3(dt, 0.0f, 0.0f));
	//TGen::Matrix4x4 rot = TGen::Matrix4x4::RotationX(TGen::Radian(dt * 0.1));
	
	//sceneSubsystem.getSceneRoot().getChild("test5")->setOrientation(rot * TGen::Vector3(sceneSubsystem.getSceneRoot().getChild("test5")->getLocalOrientation()));
	//sceneSubsystem.getSceneRoot().getChild("testmap")->setOrientation(rot * TGen::Vector3(sceneSubsystem.getSceneRoot().getChild("testmap")->getLocalOrientation()));
	// TODO: add lights IN the scene node of the map
	
	inventorySubsystem.update(dt);
	controllerSubsystem.update(dt);				// perform controlling
	physicsSubsystem.update(dt);					
	sceneSubsystem.update(dt);	
	soundSubsystem.update(dt);
}

void TGen::Engine::World::updateListener(const TGen::Vector3 & position, const TGen::Vector3 & velocity, const TGen::Vector3 & forward, const TGen::Vector3 & up) {
	soundSubsystem.setListener(position, velocity, forward, up);	
}

TGen::Color TGen::Engine::World::getAmbientLight() {
	return TGen::Color(0.4, 0.4, 0.4, 1.0);
}

TGen::Engine::PlayerController * TGen::Engine::World::getPlayerController(const std::string & name) {
	return controllerSubsystem.getController(name);
}


/*TGen::Engine::Player * TGen::Engine::World::createPlayer() {
	TGen::Camera * camera = new TGen::Camera("maincam", TGen::Vector3(0.0f, 2.0f, 0.0f), TGen::Rotation::Identity);
	sceneSubsystem.getSceneRoot().addChild(camera);
	
	// TODO: ange inte playerstuff i entities, men player ska vara en TGen::Engine::Entity!
	//       dvs, populera entiteten med components i ctor eftersom man vill länka och stå i.
	//       består av en eller flera scenenodes, en kamera, ljud möjligtvis, phsyGeom, physBody
	//			healthComponent, ...
	//       ska kunna finnas flera olika sorters players, ghost, physplayer osv. därför det är viktigt med getPlayerController
	//       men mest logiskt vore om man fick controllern från game, det är ju den som bestämmer om man ska ha ghostcam eller
	//       vara i gubben.
	
	// Game.getPlayerController!!!!!! sen frågar den world efter olika controllers beroende på vilken mode game är i.
	//			typ world.getController("ghostplayer"), world.getController("physplayer").
	//			eftersom en controller är kopplad till en spelare så måste man byta controller när man byter spelare, 
	//			riktig spelare -> ghost t ex
	// Game.getPlayerCam = const Camera *. frågar också världen efter den korrekta kameran
	
	//{ 1.0f, 2.0f, 1.0f, 2.0f, 4.0f, 2.0f, 1.0f, 2.0f, 1.0f} gaussian
	
	// TODO: entitysystemet behöver kunna sätta ihop olika modeller på joints.
	
	// TODO: world borde ge playercontroller till inputmapper!!!!! Sen inputMapper.update(dt, world.getPlayerController())!
	
 // DERIVE FPSCONTROLLER. FirstPersonController
 
	return NULL;
}*/
// rendreraren ber world att uppdatera sig internt för en viss kamera, sen frågar den efter renderlist, lights, osv
