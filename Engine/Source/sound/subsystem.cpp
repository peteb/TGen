/*
 *  subsystem.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "sound/subsystem.h"
#include "sound/soundsource.h"
#include "sound/globalsource.h"
#include "sound/localsource.h"
#include "sound/sound.h"
#include "sound/localrecipe.h"
#include "sound/soundresource.h"
#include "sound/sourcescript.h"

#include "log.h"
#include "generateline.h"
#include "fmod/fmod_errors.h"
#include "filesystem.h"
#include "file.h"
#include "staticcomponentrecipe.h"
#include "entity.h"

using TGen::Engine::Sound::GlobalSource;
using TGen::Engine::Sound::LocalSource;

TGen::Engine::Filesystem * TGen::Engine::Sound::Subsystem::filesystem = NULL;

TGen::Engine::Sound::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & fs) 
	: logs(logs)
	, fmodSystem(NULL)
	, fs(fs)
{
	logs.info["sound+"] << "*** INITIALIZING SOUND ***" << TGen::endl;
	
	FMOD_RESULT result;
	
	result = FMOD::System_Create(&fmodSystem);
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem", "Failed to create fmod system: ") << FMOD_ErrorString(result) << " (" << result << ")";
	
	result = fmodSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem", "Failed to initialize fmod system: ") << FMOD_ErrorString(result) << " (" << result << ")";
	
	filesystem = &fs;
	fmodSystem->setFileSystem(&openCallback, &closeCallback, &readCallback, &seekCallback, -1);
	
	logs.info["sound+"] << "fmod initialized" << TGen::endl;
	
}


TGen::Engine::Sound::Subsystem::~Subsystem() {
	logs.info["sound-"] << "*** SHUTTING DOWN SOUND ***" << TGen::endl;
	
	for (SoundMap::iterator iter = sounds.begin(); iter != sounds.end(); ++iter)
		delete iter->second;
	
	//for (int i = 0; i < localSources.size(); ++i)
	//	delete localSources[i];
	
	fmodSystem->release();
}


TGen::Engine::Component * TGen::Engine::Sound::Subsystem::createComponent(const std::string & name, TGen::Engine::Entity & entity, const TGen::PropertyTree & properties) {
	std::string filename = properties.getProperty("sound", "");
	TGen::Engine::Sound::Source * ret = NULL;
	
	if (properties.getName() == "soundLocal") {
		LocalSource * newSource = new LocalSource(name, filename, properties.getProperty("link", "sceneNode"), *this);
		newSource->setMinMaxDistance(TGen::lexical_cast<scalar>(properties.getProperty("minDistance", "4.0")),
											  TGen::lexical_cast<scalar>(properties.getProperty("maxDistance", "10000.0")));
		localSources.push_back(newSource);
		ret = newSource;
	}
	else if (properties.getName() == "soundGlobal") {
		GlobalSource * newSource = new GlobalSource(name, filename, *this);
		globalSources.push_back(newSource);
		ret = newSource;
	}
	/*else if (properties.getName() == "soundRef") {
		SoundResource * newResource = new SoundResource(name, *this);
		newResource->setSoundName(properties.getAttribute(1));
		return newResource;
	}*/
	else {
		throw TGen::RuntimeException("Sound::Subsystem::createComponent", "invalid component type: " + properties.getName());		
	}

	ret->setVolume(TGen::lexical_cast<scalar>(properties.getProperty("volume", "1.0")));

	ret->setAutoplay(TGen::lexical_cast<bool>(properties.getProperty("autoplay", "false")));
	ret->setLoop(TGen::lexical_cast<bool>(properties.getProperty("loop", "false")));
	ret->setScriptInterface(new TGen::Engine::Sound::SourceScript(name, ret, entity.getScriptInterface()));
	
	return ret;	
}


TGen::Engine::ComponentRecipe * TGen::Engine::Sound::Subsystem::createComponentRecipe(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	std::cout << "RECIPE: " << name << std::endl;

	/*if (properties.getName() == "soundRef") {
		return new TGen::Engine::StaticComponentRecipe(name, createComponent(name, entityName, properties));
	}
	else */if (properties.getName() != "soundLocal") {
		throw TGen::RuntimeException("Sound::Subsystem::createComponentRecipe", "can only create local sounds with recipes, failed at " + properties.getName());
	}
	
	std::string filename = properties.getProperty("sound", "");
	 
	LocalSource * prototypeSource = new LocalSource(name, filename, properties.getProperty("link", "sceneNode"), *this);
	
	prototypeSource->setMinMaxDistance(TGen::lexical_cast<scalar>(properties.getProperty("minDistance", "1.0")),
													TGen::lexical_cast<scalar>(properties.getProperty("maxDistance", "10000.0")));
	 
	prototypeSource->setAutoplay(TGen::lexical_cast<bool>(properties.getProperty("autoplay", "false")));
	prototypeSource->setLoop(TGen::lexical_cast<bool>(properties.getProperty("loop", "false")));
	prototypeSource->setPrototype(true);
	 
	localSources.push_back(prototypeSource);
	
	
	
	TGen::Engine::Sound::LocalRecipe * newRecipe = new TGen::Engine::Sound::LocalRecipe(name, *this);
	
	
	
	newRecipe->setSoundPrototype(prototypeSource);
	newRecipe->setLinkWith(properties.getProperty("link", "sceneNode"));
	
	
	
	return newRecipe;
}


void TGen::Engine::Sound::Subsystem::update(scalar dt) {	
	for (int i = 0; i < localSources.size(); ++i)
		localSources[i]->update(dt);
	
	for (int i = 0; i < globalSources.size(); ++i)
		globalSources[i]->update(dt);
		
	fmodSystem->update();
}

/*FMOD::System * TGen::Engine::Sound::Subsystem::getFmodSystem() {
	return fmodSystem;
}*/


void TGen::Engine::Sound::Subsystem::setListener(const TGen::Vector3 & position, const TGen::Vector3 & velocity, const TGen::Vector3 & forward, const TGen::Vector3 & up) {
	FMOD_VECTOR p, v, f, u;
	p.x = position.x;
	p.y = position.y;
	p.z = position.z;
	
	v.x = velocity.x;
	v.y = velocity.y;
	v.z = velocity.z;
	
	f.x = forward.x;
	f.y = forward.y;
	f.z = forward.z;
	
	u.x = forward.x;
	u.y = forward.y;
	u.z = forward.z;
	
	//std::cout << "vel: " << velocity.getMagnitude() << " m/s (" << velocity.getMagnitude() * 3.6 << " km/h) pos: " << std::string(position) << std::endl;
	fmodSystem->set3DListenerAttributes(0, &p, &v, &f, &u);
}


TGen::Engine::Sound::Sound * TGen::Engine::Sound::Subsystem::getSound(const std::string & name) {
	logs.info["sound"] << "request for sound \"" << name << "\"" << TGen::endl;

	std::string fixedName;
	
	if (name.substr(0, 7) != "http://")
		fixedName = TGen::prependPath(name, "/sounds/");
	else
		fixedName = name;
	
	TGen::Engine::Sound::Sound * ret = NULL;
	
	SoundMap::iterator iter = sounds.find(fixedName);
	if (iter != sounds.end()) {
		ret = iter->second;
		
		if (ret->isStream())
			throw TGen::RuntimeException("Sound::Subsystem::getSound", "Can't reuse stream sound");
	}
	else {
		logs.info["sound"] << "   not loaded, loading..." << TGen::endl;		

		TGen::Engine::GenerateLine genline("gen:" + fixedName);

		ret = createSound(genline);
		sounds.insert(std::make_pair(fixedName, ret));
	}
	
	std::cout << "CREATED SOUND " << ret << std::endl;
		
	return ret;
}

void TGen::Engine::Sound::Subsystem::addLocalSource(TGen::Engine::Sound::LocalSource * source) {
	localSources.push_back(source);
}

TGen::Engine::Sound::Sound * TGen::Engine::Sound::Subsystem::createSound(const TGen::Engine::GenerateLine & genline) {
	FMOD::Sound * ret = NULL;
	
	bool stream = false;
	std::string filename;
		
	/*if (genline.getName().substr(0, 7) == "stream:") {
		stream = true;
		filename = genline.getName().substr(7);
	}
	else {
		filename = genline.getName();
	}*/
	
	filename = genline.getName();
	
	FMOD_MODE flags = FMOD_DEFAULT;
	FMOD_RESULT result;
	
	std::string loopMode = genline.getParameter("loop", "off");
	
	if (loopMode == "normal")
		flags |= FMOD_LOOP_NORMAL;
	else if (loopMode == "bidi")
		flags |= FMOD_LOOP_BIDI;
	else {
		try {
			if (TGen::lexical_cast<bool>(genline.getParameter("loop", "false")))
				flags |= FMOD_LOOP_NORMAL;
		}
		catch (...) {
			
		}
	}
	
	stream = TGen::lexical_cast<bool>(genline.getParameter("stream", "false"));
	

	if (TGen::lexical_cast<bool>(genline.getParameter("software", "true")))
		flags |= FMOD_SOFTWARE;
	else
		flags |= FMOD_HARDWARE;

	if (TGen::lexical_cast<bool>(genline.getParameter("net", "false")))
		fmodSystem->setFileSystem(NULL, NULL, NULL, NULL, -1);
	else
		fmodSystem->setFileSystem(&openCallback, &closeCallback, &readCallback, &seekCallback, -1);

	
	if (stream)
		result = fmodSystem->createStream(filename.c_str(), flags, NULL, &ret);
	else
		result = fmodSystem->createSound(filename.c_str(), flags, NULL, &ret);
	
	
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem::createSound", "failed to load " + std::string(stream ? "stream " : "") + "sound '" + filename + "': ") << FMOD_ErrorString(result);
	
	return new TGen::Engine::Sound::Sound(ret, stream);
}

void TGen::Engine::Sound::Subsystem::link() {
	for (int i = 0; i < localSources.size(); ++i) {
		localSources[i]->linkSubsystem(*this);
	}
	
	for (int i = 0; i < globalSources.size(); ++i) {
		globalSources[i]->linkSubsystem(*this);		
	}
}

// TODO: varför funkar inte transform på musiklådan?
// TODO: riktig ljudhantering, ladda inte in ljudet varje gång!! resourcemanager!
// TODO: räkna ut normaler manuellt i md3-inladdningen.
// TODO: rätt metarendrering, man kan antingen stänga av eller sätta igång allt. vill kunna tuna!

FMOD_RESULT F_CALLBACK TGen::Engine::Sound::Subsystem::openCallback(const char * name, int unicode, unsigned int * filesize, void ** handle, void ** userdata) {	
	if (!filesystem)
		throw TGen::RuntimeException("Sound::Subsystem::openCallback", "no filesystem! I hate globals.");
	
	try {
		TGen::Engine::File * file(filesystem->openRead(name));
	
		*reinterpret_cast<TGen::Engine::File **>(handle) = file;
		*filesize = file->getSize();
	}
	catch (...) {
		return FMOD_ERR_FILE_NOTFOUND;
	}
	
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK TGen::Engine::Sound::Subsystem::closeCallback(void * handle, void * data) {
	delete reinterpret_cast<TGen::Engine::File *>(handle);
	
	return FMOD_OK;	
}

FMOD_RESULT F_CALLBACK TGen::Engine::Sound::Subsystem::readCallback(void * handle, void * buffer, unsigned int sizebytes, unsigned int * bytesread, void * userdata) {
	TGen::Engine::File * file = reinterpret_cast<TGen::Engine::File *>(handle);
	
	*bytesread = file->read(reinterpret_cast<char *>(buffer), sizebytes);
	
	//std::cout << "READ: " << *bytesread << " OF " << sizebytes << std::endl;
	
	if (*bytesread < sizebytes)
		return FMOD_ERR_FILE_EOF;
	
	return FMOD_OK;	
}

FMOD_RESULT F_CALLBACK TGen::Engine::Sound::Subsystem::seekCallback(void * handle, unsigned int pos, void * userdata) {
	TGen::Engine::File * file = reinterpret_cast<TGen::Engine::File *>(handle);
	std::cout << "SEEK " << pos << std::endl;
	
	file->seekReadPos(pos, TGen::beg);
	
	return FMOD_OK;	
}



