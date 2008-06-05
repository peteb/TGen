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
	
	result = fmodSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem", "Failed to initialize fmod system: ") << FMOD_ErrorString(result) << " (" << result << ")";
	
	filesystem = &fs;
	fmodSystem->setFileSystem(&openCallback, &closeCallback, &readCallback, &seekCallback, -1);
	
	logs.info["snd+"] << "fmod initialized" << TGen::endl;
	
}

TGen::Engine::Sound::Subsystem::~Subsystem() {
	logs.info["snd-"] << "*** SHUTTING DOWN SOUND ***" << TGen::endl;
	
	for (SoundSourceList::iterator iter = soundSources.begin(); iter != soundSources.end(); ++iter)
		delete *iter;
	
	fmodSystem->release();
}


TGen::Engine::Component * TGen::Engine::Sound::Subsystem::createComponent(const std::string & name, const std::string & entityName, const TGen::PropertyTree & properties) {
	bool stream = false;
	std::string filename = properties.getProperty("file", "unknown");
	
	if (filename.substr(0, 7) == "stream:") {
		stream = true;
		filename = filename.substr(7);
	}
	
	if (properties.getName() == "sndLocal") {
		
	}
	else if (properties.getName() == "sndGlobal") {
		std::auto_ptr<TGen::Engine::Sound::GlobalSource> newSource(new TGen::Engine::Sound::GlobalSource(name, filename, stream));
		
		// en channel skapas antingen med en FMOD_Channel eller en string..
		globalSources.push_back(newSource.get());

		return newSource.release();
	}
	else {
		throw TGen::RuntimeException("Sound::Subsystem::createComponent", "invalid component type: " + properties.getName());
	}
	
	/*std::string fileName = properties.getProperty("file", "");
	std::string streamName = properties.getProperty("stream", "");
	
	bool positional = TGen::lexical_cast<bool>(properties.getProperty("positional", "false"));
	bool threedee = positional;
	bool hardware = false;
	bool loop = TGen::lexical_cast<bool>(properties.getProperty("loop", "false"));
	
	
	
	FMOD::Sound * sound;
	FMOD_RESULT result;
	FMOD_MODE flags = FMOD_INIT_3D_RIGHTHANDED;
	
	if (hardware)
		flags |= FMOD_HARDWARE;
	else
		flags |= FMOD_SOFTWARE;
	
	
	if (threedee)
		flags |= FMOD_3D;
	else
		flags |= FMOD_2D;
	
	if (loop)
		flags |= FMOD_LOOP_NORMAL;
	else
		flags |= FMOD_LOOP_OFF;
	
	
	if (!fileName.empty())
		result = fmodSystem->createSound(fileName.c_str(), flags, 0, &sound);
	else if (!streamName.empty())
		result = fmodSystem->createStream(streamName.c_str(), flags, 0, &sound);
	else
		throw TGen::RuntimeException("Sound::Subsystem::createComponent", "no file or stream specified");
	
	if (result != FMOD_OK)
		throw TGen::RuntimeException("Sound::Subsystem::createComponent", FMOD_ErrorString(result));
	
	if (positional) {
		sound->set3DMinMaxDistance(TGen::lexical_cast<scalar>(properties.getProperty("minDistance", "1.0")),
											TGen::lexical_cast<scalar>(properties.getProperty("maxDistance", "10000.0")));
	}
	
	TGen::Engine::Sound::SoundSource * newSource(new TGen::Engine::Sound::SoundSource(name, sound, *this, properties.getProperty("link", "sceneNode")));
	
	newSource->setSingleChannel(TGen::lexical_cast<bool>(properties.getProperty("singleChannel", "false")));
	
	if (TGen::lexical_cast<bool>(properties.getProperty("play", "true")))
		newSource->play();
	
	soundSources.push_back(newSource);
	
	//throw TGen::RuntimeException("blah", "blah " + properties.getProperty("file", "bla"));
	
	return newSource;*/
}

void TGen::Engine::Sound::Subsystem::update(scalar dt) {
	for (SoundSourceList::iterator iter = soundSources.begin(); iter != soundSources.end(); ++iter) {
		(*iter)->update(dt);
	}
	
	fmodSystem->update();
}

FMOD::System * TGen::Engine::Sound::Subsystem::getFmodSystem() {
	return fmodSystem;
}

void TGen::Engine::Sound::Subsystem::createChannels(TGen::Engine::Sound::Source & source, const TGen::PropertyTree & properties) {
	for (int i = 0; i < properties.getNumNodes(); ++i) {
		const TGen::PropertyTree & channel = properties.getNode(i);
		
		if (channel.getName() == "channel") {
			
		}
	}
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


FMOD::Sound * TGen::Engine::Sound::Subsystem::getStream(const std::string & name) {
	FMOD::Sound * ret = NULL;

	logs.info["snd"] << "request for stream \"" << name << "\"" << TGen::endl;
	
	SoundMap::iterator iter = streams.find(name);
	if (iter != streams.end()) {
		ret = iter->second;
	}
	else {
		logs.info["snd"] << "   not loaded, loading..." << TGen::endl;

		FMOD_MODE flags = 0;
		
		/*if (hardware)
		 flags |= FMOD_HARDWARE;
		 else
		 flags |= FMOD_SOFTWARE;
		 */
		
		if (false)
			flags |= FMOD_3D;
		else
			flags |= FMOD_2D;
		
		if (false)
			flags |= FMOD_LOOP_NORMAL;
		else
			flags |= FMOD_LOOP_OFF;
		
		
		FMOD_RESULT result = fmodSystem->createStream(name.c_str(), flags, 0, &ret);
		// TODO: get flags from genline.. loop normal hardware true positional true
		
		if (result != FMOD_OK)
			throw TGen::RuntimeException("Sound::Subsystem::getStream", "Failed to get stream '" + name + "': " + FMOD_ErrorString(result));
		
		
	}
	
	return ret;
}

FMOD::Sound * TGen::Engine::Sound::Subsystem::getSound(const std::string & name) {
	FMOD::Sound * ret = NULL;

	logs.info["snd"] << "request for sound \"" << name << "\"" << TGen::endl;
	
	SoundMap::iterator iter = sounds.find(name);
	if (iter != sounds.end()) {
		ret = iter->second;
	}
	else {
		logs.info["snd"] << "   not loaded, loading..." << TGen::endl;		
		
	}
	
	return ret;
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



