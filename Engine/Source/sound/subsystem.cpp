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

#include "log.h"
#include "generateline.h"
#include "fmod/fmod_errors.h"
#include "filesystem.h"
#include "file.h"

TGen::Engine::Filesystem * TGen::Engine::Sound::Subsystem::filesystem = NULL;

TGen::Engine::Sound::Subsystem::Subsystem(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & fs) 
	: logs(logs)
	, fmodSystem(NULL)
	, fs(fs)
{
	logs.info["snd+"] << "*** INITIALIZING SOUND ***" << TGen::endl;
	
	FMOD_RESULT result;
	
	result = FMOD::System_Create(&fmodSystem);
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem", "Failed to create fmod system: ") << FMOD_ErrorString(result) << " (" << result << ")";
	
	result = fmodSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem", "Failed to initialize fmod system: ") << FMOD_ErrorString(result) << " (" << result << ")";
	
	filesystem = &fs;
	fmodSystem->setFileSystem(&openCallback, &closeCallback, &readCallback, &seekCallback, -1);
	
	logs.info["snd+"] << "fmod initialized" << TGen::endl;
	
}

TGen::Engine::Sound::Subsystem::~Subsystem() {
	logs.info["snd-"] << "*** SHUTTING DOWN SOUND ***" << TGen::endl;
	
	//for (SoundSourceList::iterator iter = soundSources.begin(); iter != soundSources.end(); ++iter)
	//	delete *iter;
	
	for (SoundMap::iterator iter = sounds.begin(); iter != sounds.end(); ++iter)
		delete iter->second;
	
	for (int i = 0; i < localSources.size(); ++i)
		delete localSources[i];
	
	fmodSystem->release();
}


TGen::Engine::Component * TGen::Engine::Sound::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	std::string filename = properties.getProperty("file", "unknown");
	TGen::Engine::Sound::Source * ret = NULL;
	
	/*if (properties.getName() == "sndLocal") {
		
	}
	else */if (properties.getName() == "sndGlobal") {
		TGen::Engine::Sound::GlobalSource * newSource = new TGen::Engine::Sound::GlobalSource(name, filename);
		
		globalSources.push_back(newSource);

		ret = newSource;
	}
	else {
		throw TGen::RuntimeException("Sound::Subsystem::createComponent", "invalid component type: " + properties.getName());		
	}
	
	ret->setAutoplay(TGen::lexical_cast<bool>(properties.getProperty("autoplay", "false")));
	ret->setLoop(TGen::lexical_cast<bool>(properties.getProperty("loop", "false")));
	
	return ret;	
}

void TGen::Engine::Sound::Subsystem::update(scalar dt) {
	for (int i = 0; i < localSources.size(); ++i)
		localSources[i]->update(dt);
	
	for (int i = 0; i < globalSources.size(); ++i)
		globalSources[i]->update(dt);
		
	fmodSystem->update();
}

FMOD::System * TGen::Engine::Sound::Subsystem::getFmodSystem() {
	return fmodSystem;
}


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
	logs.info["snd"] << "request for sound \"" << name << "\"" << TGen::endl;

	
	TGen::Engine::Sound::Sound * ret = NULL;
	
	SoundMap::iterator iter = sounds.find(name);
	if (iter != sounds.end()) {
		ret = iter->second;
	}
	else {
		logs.info["snd"] << "   not loaded, loading..." << TGen::endl;		

		TGen::Engine::GenerateLine genline("gen:" + name);

		ret = createSound(genline);
		sounds.insert(std::make_pair(name, ret));
	}
		
	return ret;
}

TGen::Engine::Sound::Sound * TGen::Engine::Sound::Subsystem::createSound(const TGen::Engine::GenerateLine & genline) {
	FMOD::Sound * ret = NULL;
	
	bool stream = false;
	std::string filename;
	
	if (genline.getName().substr(0, 7) == "stream:") {
		stream = true;
		filename = genline.getName().substr(7);
	}
	else {
		filename = genline.getName();
	}
	
	FMOD_MODE flags = FMOD_DEFAULT;
	FMOD_RESULT result;
	
	if (stream)
		result = fmodSystem->createStream(filename.c_str(), flags, NULL, &ret);
	else
		result = fmodSystem->createSound(filename.c_str(), flags, NULL, &ret);
	
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem::createSound", "failed to load sound '" + filename + "': ") << FMOD_ErrorString(result);
	
	return new TGen::Engine::Sound::Sound(ret);
}

void TGen::Engine::Sound::Subsystem::link() {
	for (int i = 0; i < localSources.size(); ++i) {
		localSources[i]->link(*this);
	}
	
	for (int i = 0; i < globalSources.size(); ++i) {
		globalSources[i]->link(*this);		
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



