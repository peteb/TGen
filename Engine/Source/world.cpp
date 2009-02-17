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
#include "info/worldinfo.h"

#include <OpenGL/OpenGL.h>
#include <SDL/SDL.h>

TGen::Engine::World::World(TGen::Engine::Filesystem & filesystem, TGen::Engine::ResourceManager & resources, TGen::Engine::StandardLogs & logs, TGen::VertexDataSource & dataSource, const std::string & mapname)
	: filesystem(filesystem)
	, logs(logs)
	, sceneSubsystem(resources, filesystem, logs, dataSource)
	, physicsSubsystem(logs, filesystem)
	, soundSubsystem(logs, filesystem)
	, scriptSubsystem(logs, filesystem, mapname)
	, mainCam(NULL)
	, lightList(100)
	, entityFactory(logs)
	, worldInfo(NULL)
{
	logs.info["world+"] << "initializing world '" << mapname << "'..." << TGen::endl;
	
	logs.info["world+"] << "   registering subsystems..." << TGen::endl;
	
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
	//entityFactory.registerSubsystem("soundRef", &soundSubsystem);
	
	entityFactory.registerSubsystem("inventory", &inventorySubsystem);
	entityFactory.registerSubsystem("weapon", &inventorySubsystem);
	
//	entityFactory.registerSubsystem("event", &scriptSubsystem);
	//entityFactory.registerSubsystem("-event", &scriptSubsystem);
	entityFactory.registerSubsystem("script", &scriptSubsystem);
	
	entityFactory.registerSubsystem("worldInfo", &infoSubsystem);
	
	entityFactory.registerSubsystem("timer", &utilsSubsystem);
	//entityFactory.registerSubsystem("objectRef", &utilsSubsystem);
	//entityFactory.registerSubsystem("prototypeRef", &utilsSubsystem);
	
	// TODO: this class is a hog
	// TODO: sen i fysikmotorn borde man kunna låsa de objekt som inte är i något aktuellt rum, slippa uppdatera en massa. borde dock följa med hierarkiskt.
	// TODO: kunna pruna ett materials resurser, men om de används på andra ställen då? då måste refcount in i bilden...

	
	
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapBuffers();
	
	
	
	infoSubsystem.setWorld(this);

	logs.info["world+"] << "   preparing filesystem..." << TGen::endl;
	filesystem.addSearchPath(filesystem.getRealPath("/maps/" + mapname), true);
	
	logs.info["world+"] << "   loading definitions..." << TGen::endl;
	loadDefinitions("/definitions/");	
	
	logs.info["world+"] << "   loading entities..." << TGen::endl;
	//loadDefinitions("/maps/" + mapname + "/entities/");
	
	loadDefinitions("/entities/");		// TODO: this should be the only entities folder
	
	//if (entities.numEntities() == 0)
	//	throw TGen::RuntimeException("World::World", "No entities defined; invalid filesystem for \"" + mapname + "\".");
	
	
	logs.info["world+"] << "   linking entities" << TGen::endl;
	entities.link();
	
	logs.info["world+"] << "   linking and updating scene..." << TGen::endl;
	sceneSubsystem.link();
	sceneSubsystem.update(0.0f);
	
	logs.info["world+"] << "   linking sound subsystem..." << TGen::endl;
	soundSubsystem.link();
	
	logs.info["world+"] << "   linking physics subsystem..." << TGen::endl;
	physicsSubsystem.link();
	
	scriptSubsystem.executeScripts("/scripts/");
	logs.info["world+"] << "   world created" << TGen::endl;
}


void TGen::Engine::World::loadEntities(const std::string & filename) {
	std::auto_ptr<TGen::Engine::File> entitiesFile(filesystem.openRead(filename));
	
	TGen::PropertyTreeParser propParser;
	TGen::PropertyTree props = propParser.parse(entitiesFile->readAll().c_str());
	
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


void TGen::Engine::World::loadDefinitions(const std::string & path) {
	std::vector<std::string> files;
	files.reserve(50);
	
	filesystem.enumerateFiles(path, files, true);
	
	for (int i = 0; i < files.size(); ++i) {
		//try {
			loadEntities(files[i]);
		//}
		//catch (const TGen::RuntimeException & error) {
			//logs.warning["init"] << "world: failed to load definition files: " << error << TGen::endl;
		//}
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
	// TODO: add lights IN the scene node of the map
	

	utilsSubsystem.update(dt);

	inventorySubsystem.update(dt);

	sceneSubsystem.update(dt);	

	controllerSubsystem.update(dt);				// perform controlling
	physicsSubsystem.update(dt);					
	soundSubsystem.update(dt);
}

/*
	Hur får world sin egna entitet? World SKA hanteras som en vanlig entitet, men den här world (objektet vi är i nu) ska bara peka på den world. WorldEntity
 
	Om någon entitet har en worldInfo så länkas World automatiskt med den, throwa om redan länkad
	
	För att ändra bana... [game changeMap: "hey.map"];
 
	[world setPlayerView: playerController];
	[world setAmbient: 2 3 1];
		
	world {
		worldInfo {
			ambientLight "0 0 0"
			playerView "player_start:hellote"
		}
 
		event init {
			[worldInfo setPlayerView: @stuff];
		}
	}
 
 */

void TGen::Engine::World::updateListener(const TGen::Vector3 & position, const TGen::Vector3 & velocity, const TGen::Vector3 & forward, const TGen::Vector3 & up) {
	soundSubsystem.setListener(position, velocity, forward, up);	
}

TGen::Color TGen::Engine::World::getAmbientLight() {
	if (worldInfo)
		return worldInfo->getAmbientLight();
	
	return TGen::Color(0.4, 0.4, 0.4, 1.0);
}

TGen::Engine::PlayerController * TGen::Engine::World::getPlayerController() {
	TGen::Engine::PlayerController * ret = NULL;
	
	if (worldInfo)
		ret = worldInfo->getPlayerController();
	
	return ret; 
}

TGen::Engine::Scene::Node * TGen::Engine::World::getPlayerCamera() {
	//TGenAssert(getPlayerController());
	if (!getPlayerController())
		return NULL;
	
	return getPlayerController()->getActiveCamera();
}


void TGen::Engine::World::setWorldInfo(TGen::Engine::Info::WorldInfo * worldInfo) {
	if (this->worldInfo)
		throw TGen::RuntimeException("World::setWorldInfo", "worldInfo already set!");
	
	this->worldInfo = worldInfo;
}


/*TGen::Engine::Player * TGen::Engine::World::createPlayer() {
	TGen::Camera * camera = new TGen::Camera("maincam", TGen::Vector3(0.0f, 2.0f, 0.0f), TGen::Rotation::Identity);
	sceneSubsystem.getSceneRoot().addChild(camera);
	
	
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
