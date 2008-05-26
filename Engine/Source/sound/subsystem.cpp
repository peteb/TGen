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
#include "log.h"
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
	
	result = fmodSystem->init(100, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem", "Failed to initialize fmod system: ") << FMOD_ErrorString(result) << " (" << result << ")";
	
	filesystem = &fs;
	fmodSystem->setFileSystem(&openCallback, &closeCallback, &readCallback, &seekCallback, -1);
	
	logs.info["snd+"] << "fmod initialized" << TGen::endl;
	
}

TGen::Engine::Sound::Subsystem::~Subsystem() {
	logs.info["snd-"] << "*** SHUTTING DOWN SOUND ***" << TGen::endl;
}


TGen::Engine::Component * TGen::Engine::Sound::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	std::string fileName = properties.getProperty("file", "");
	std::string streamName = properties.getProperty("stream", "");
	
	FMOD::Sound * sound;
	FMOD_RESULT result;
	
	if (!fileName.empty())
		result = fmodSystem->createSound(fileName.c_str(), FMOD_DEFAULT, 0, &sound);
	else if (!streamName.empty())
		result = fmodSystem->createStream(streamName.c_str(), FMOD_DEFAULT, 0, &sound);
	else
		throw TGen::RuntimeException("Sound::Subsystem::createComponent", "no file or stream specified");
	
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem::createComponent", FMOD_ErrorString(result));
	
	/*FMOD::Channel * channel;
	result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);

	if (result != FMOD_OK)
		throw TGen::RuntimeException("BOOOOOOOM", "BAAAAM");
	*/
	
	TGen::Engine::Sound::SoundSource * newSource(new TGen::Engine::Sound::SoundSource(name, sound));
	
	soundSources.push_back(newSource);
	
	//throw TGen::RuntimeException("blah", "blah " + properties.getProperty("file", "bla"));
	
	return newSource;
}

void TGen::Engine::Sound::Subsystem::update(scalar dt) {
	for (SoundSourceList::iterator iter = soundSources.begin(); iter != soundSources.end(); ++iter) {
		(*iter)->update(dt);
	}
}




FMOD_RESULT F_CALLBACK TGen::Engine::Sound::Subsystem::openCallback(const char * name, int unicode, unsigned int * filesize, void ** handle, void ** userdata) {	
	if (!filesystem)
		throw TGen::RuntimeException("Sound::Subsystem::openCallback", "no filesystem! I hate globals.");
	
	TGen::Engine::File * file(filesystem->openRead(name));
	
	*reinterpret_cast<TGen::Engine::File **>(handle) = file;
	*filesize = file->getSize();
	
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



