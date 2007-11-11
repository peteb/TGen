/*
 *  resourcemanager.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "resourcemanager.h"
#include "log.h"
#include "filesystem.h"
#include "file.h"
#include "meshgenerator.h"

TGen::Engine::ResourceManager::ResourceManager(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem, TGen::Renderer & renderer) 
	: logs(logs)
	, filesystem(filesystem)
	, renderer(renderer)
{
	logs.info["res+"] << "..." << TGen::endl;
}

TGen::Engine::ResourceManager::~ResourceManager() {
	logs.info["res-"] << "removing meshes..." << TGen::endl;
	for (MeshMap::iterator iter = meshes.begin(); iter != meshes.end(); ++iter)
		delete iter->second;
	
	logs.info["res-"] << "removing textures..." << TGen::endl;
	for (TextureMap::iterator iter = textures.begin(); iter != textures.end(); ++iter)
		delete iter->second;
	
	logs.info["res-"] << "removing shaders..." << TGen::endl;
	for (ShaderMap::iterator iter = shaders.begin(); iter != shaders.end(); ++iter)
		delete iter->second;
	
	
}

TGen::ShaderProgram * TGen::Engine::ResourceManager::getShaderProgram(const std::string & name) {
	ShaderMap::iterator iter = shaders.find(name);
	if (iter != shaders.end())
		return iter->second;
	
	std::string fixedName = "/shaders/" + name;
	
	logs.info["res"] << "loading shader '" << fixedName << "'..." << TGen::endl;
	
	TGen::Engine::File * file = filesystem.openRead(fixedName);
	std::string contents = file->readAll();
	delete file;

	TGen::ShaderProgram * ret = renderer.createShaderProgram(contents.c_str());
	ret->link();
	
	return ret;
}

TGen::Texture * TGen::Engine::ResourceManager::getTexture(const std::string & name) {
	
	return NULL;
}

TGen::Mesh * TGen::Engine::ResourceManager::getMesh(const std::string & name) {
	// hur ska man ladda in fillmeshen? kan ju ha en meshgenerator som kan skapa lite olika meshes
	// getMesh("gen:quad -1 -1 1 1");
	// getMesh("gen:fillquad");
	// MeshGenerator hey; hey.generate("fillquad") -> MeshMap; return;
	
	MeshMap::iterator iter = meshes.find(name);
	if (iter != meshes.end())
		return iter->second;
		
	TGen::Mesh * newMesh = NULL;
	
	if (name.substr(0, strlen("gen:")) == "gen:") {
		TGen::Engine::MeshGenerator generator;
		newMesh = generator.generateMesh(name.substr(strlen("gen:"), name.size() - strlen("gen:")));
	}
	else {
		
	}
	
	meshes[name] = newMesh;
	
	return newMesh;
}

int TGen::Engine::ResourceManager::getTextureType(const std::string & name) {
	// return 1 if the type is sent to the material as aux
	
	return 0;
}

