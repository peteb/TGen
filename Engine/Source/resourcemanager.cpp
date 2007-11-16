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
	logs.info["res+"] << "initializing resource manager..." << TGen::endl;
	
	loadMaterials("/materials/stuff.material");
	loadMaterials("/materials/deferred.material");
	loadMaterials("/materials/test.material");

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
	
	logs.info["res-"] << "removing materials..." << TGen::endl;
	for (MaterialMap::iterator iter = materials.begin(); iter != materials.end(); ++iter)
		delete iter->second;
	
	logs.info["res-"] << "shut down" << TGen::endl;
}

TGen::ShaderProgram * TGen::Engine::ResourceManager::getShaderProgram(const std::string & name) {
	ShaderMap::iterator iter = shaders.find(name);
	if (iter != shaders.end())
		return iter->second;
	
	std::string fixedName = "/shaders/" + name + ".shader";
	
	logs.info["res"] << "loading shader '" << fixedName << "'..." << TGen::endl;
	
	TGen::Engine::File * file = filesystem.openRead(fixedName);
	std::string contents = file->readAll();
	delete file;

	TGen::ShaderProgram * ret = renderer.createShaderProgram(contents.c_str());
	ret->link();
	
	shaders[name] = ret;
	
	return ret;
}

TGen::Texture * TGen::Engine::ResourceManager::getTexture(const std::string & name) {
	
	return NULL;
}

int TGen::Engine::ResourceManager::getTextureType(const std::string & name) {
	// return 1 if the type is sent to the material as aux
	
	return 0;
}

TGen::Mesh * TGen::Engine::ResourceManager::getMesh(const std::string & name) {
	// hur ska man ladda in fillmeshen? kan ju ha en meshgenerator som kan skapa lite olika meshes
	// getMesh("gen:quad -1 -1 1 1");
	// getMesh("gen:fillquad");
	
	MeshMap::iterator iter = meshes.find(name);
	if (iter != meshes.end())
		return iter->second;
		
	TGen::Mesh * newMesh = NULL;
	
	if (name.substr(0, strlen("gen:")) == "gen:") {
		TGen::Engine::MeshGenerator generator;
		newMesh = generator.generateMesh(name.substr(strlen("gen:"), name.size() - strlen("gen:")), renderer);
	}
	else {
		// TODO: checka filformat för parser
		TGen::Engine::File * file = filesystem.openRead(name);
		TGen::MD3::Parser modelParser;
		TGen::MD3::File * md3File = modelParser.parse(*file);
		delete file;
		
		//md3File->printInfo(std::cout);
		
		newMesh = md3File->createMesh(renderer, 0.001);	// TODO: 0.001 är scale factor
		delete md3File;		
	}
	
	meshes[name] = newMesh;
	
	return newMesh;
}


TGen::Material * TGen::Engine::ResourceManager::getMaterial(const std::string & name) {
	MaterialMap::iterator iter = materials.find(name);
	
	if (iter != materials.end()) {
		TGen::Material * material = iter->second;
		
		if (!material->isLinked())
			material->link(*this);

		return material;
	}
	
	logs.warning["res"] << "material '" << name << "' is not loaded, using 'nomat'" << TGen::endl;
	
	iter = materials.find("nomat");
	if (iter != materials.end())
		return iter->second;
	
	logs.error["res"] << "material '" << name << "' is not loaded and there's no 'nomat'!" << TGen::endl;
	
	return NULL;
}

void TGen::Engine::ResourceManager::loadMaterials(const std::string & filename) {
	logs.info["res"] << "loading materials from '" << filename << "'..." << TGen::endl;
	
	std::string contents;
	TGen::Engine::File * file = filesystem.openRead(filename);
	contents = file->readAll();
	delete file;
	
	std::list<TGen::Material *> materialsLoaded;
	TGen::MaterialParser parser;
	parser.parse(contents.c_str(), materialsLoaded);
	
	logs.info["res"] << "loaded " << materialsLoaded.size() << " materials:\n";
	int i = 0;
	
	for (std::list<TGen::Material *>::iterator iter = materialsLoaded.begin(); iter != materialsLoaded.end(); ++iter) {
		MaterialMap::iterator iter2 = materials.find((*iter)->getName());
		if (iter2 != materials.end())
			delete iter2->second;
		
		materials[(*iter)->getName()] = *iter;
		logs.info["res"] << (*iter)->getName() << "  ";
		
		if (i++ > 4) {
			logs.info["res"] << "\n";
			i = 0;
		}
	}
	
	logs.info["res"] << TGen::endl;
}

