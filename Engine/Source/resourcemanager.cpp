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
#include "imagegenerator.h"
#include "devilimage.h"
#include "preprocessor.h"
#include "generateline.h"

TGen::Engine::ResourceManager::ResourceManager(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem, TGen::Renderer & renderer) 
	: logs(logs)
	, filesystem(filesystem)
	, renderer(renderer)
	, vertexCache(renderer, logs)
{
	logs.info["res+"] << "initializing resource manager..." << TGen::endl;
	
	std::vector<std::string> materialsToLoad;
	materialsToLoad.reserve(50);
	filesystem.enumerateFiles("/materials/", materialsToLoad, true);
	
	for (int i = 0; i < materialsToLoad.size(); ++i) {
		loadMaterials(materialsToLoad[i]);
	}
}

TGen::Engine::ResourceManager::~ResourceManager() {
	logs.info["res-"] << "shutting down..." << TGen::endl;

	logs.info["res-"] << "shaders: " << TGen::endl;
	for (ShaderMap::iterator iter = shaders.begin(); iter != shaders.end(); ++iter)
		logs.info["res-"] << iter->first << TGen::endl;
	
	logs.info["res-"] << "materials: " << TGen::endl;
	for (MaterialMap::iterator iter = materials.begin(); iter != materials.end(); ++iter) {
		logs.info["res-"] << iter->first << " (frames: " << iter->second->getTimesRendered() << ")" << TGen::endl;

		const TGen::Material::SpecializationList & specializations = iter->second->getSpecializations();
		for (TGen::Material::SpecializationList::const_iterator iter = specializations.begin(); iter != specializations.end(); ++iter) {
			logs.info["res-"] << "   " << iter->second->getName() << " (" << iter->second->getTimesUsed() << ")" << TGen::endl;
		}
	}
	
	logs.info["res-"] << "removing meshes..." << TGen::endl;
	for (MeshMap::iterator iter = meshes.begin(); iter != meshes.end(); ++iter) {
		logs.info["res-"] << "removing mesh " << iter->first << TGen::endl;
		delete iter->second;
	}
	
	logs.info["res-"] << "removing textures..." << TGen::endl;
	for (TextureMap::iterator iter = textures.begin(); iter != textures.end(); ++iter)
		delete iter->second;
	
	logs.info["res-"] << "removing shaders..." << TGen::endl;
	for (ShaderMap::iterator iter = shaders.begin(); iter != shaders.end(); ++iter)
		delete iter->second;
	
	logs.info["res-"] << "removing materials..." << TGen::endl;
	for (MaterialMap::iterator iter = materials.begin(); iter != materials.end(); ++iter)
		delete iter->second;
	
}

TGen::ShaderProgram * TGen::Engine::ResourceManager::getShaderProgram(const std::string & name) {
	ShaderMap::iterator iter = shaders.find(name);
	if (iter != shaders.end())
		return iter->second;

	std::string fixedName;
	bool generate = false;
		
	if (name.substr(0, 4) == "gen:") {
		int colPos = name.rfind(":");
		fixedName = "/shaders/" + name.substr(4, colPos - 4) + ".shader";
		generate = true;
	}
	else {
		fixedName = "/shaders/" + name + ".shader";	
	}
	
	logs.info["res"] << "loading shader '" << fixedName << "'..." << TGen::endl;
	
	TGen::Engine::File * file = filesystem.openRead(fixedName);
	std::string contents = file->readAll();
	delete file;

	// gen:directionalLighting:MAX_LIGHTS=1,STUFF=blabla

	
	if (generate) {
		int colPos = name.rfind(":");
		std::string genString = name.substr(colPos + 1, name.size() - colPos - 1);
		
		logs.info["res"] << "preprocessing " << fixedName << " with " << genString << TGen::endl;
		
		TGen::Engine::TextPreprocessor processor;
		contents = processor.process(contents, genString);
	}
	
	
	
	
	TGen::ShaderProgram * ret = renderer.createShaderProgram(contents.c_str());
	ret->link();
	
	shaders[name] = ret;
	
	return ret;
}

TGen::Texture * TGen::Engine::ResourceManager::getTexture(const std::string & name) {
	// gen:red:2048 2048
	
	TextureMap::iterator iter = textures.find(name);
	if (iter != textures.end())
		return iter->second;
	
	TGen::Texture * newTexture = NULL;
	
	if (name.substr(0, 4) == "gen:") {
		TGen::Engine::GenerateLine line(name);
		
		TGen::Engine::ImageGenerator generator;
		TGen::Image * newImage = generator.generateImage(line);
		
		newTexture = renderer.createTexture(*newImage, TGen::RGB);
	}
	else {
		ILuint imageName;
		ilGenImages(1, &imageName);
		ilBindImage(imageName);
	
		std::string fixedName = "textures/" + name;
	
		logs.info["res"] << "request for '" + name + "'..." << TGen::endl;
	
		TGen::Engine::File * file = filesystem.openRead(fixedName);
		TGen::Image * image = imageLoader.load(file);
		delete file;
	
		newTexture = renderer.createTexture(*image, image->getFormat());
		delete image;
	}
	
	textures[name] = newTexture;
	
	return newTexture;
}

int TGen::Engine::ResourceManager::getTextureType(const std::string & name) {
	// {NULL, colorMap, depthMap, normalMap}
	
	if (name == "$colormap")
		return 1;
	else if (name == "$normalmap" || name == "$bloom")
		return 2;
	else if (name == "$miscmap")
		return 3;
	else if (name == "$depthmap")
		return 4;
	
	return 0;
}

TGen::Mesh * TGen::Engine::ResourceManager::getMesh(const std::string & name) {
	// hur ska man ladda in fillmeshen? kan ju ha en meshgenerator som kan skapa lite olika meshes
	// getMesh("gen:quad:-1 -1 1 1");
	// getMesh("gen:fillquad");
	
	MeshMap::iterator iter = meshes.find(name);
	if (iter != meshes.end())
		return iter->second;
		
	TGen::Mesh * newMesh = NULL;
	
	if (name.substr(0, 4) == "gen:") {
		std::string generateName = name.substr(strlen("gen:"), name.size() - strlen("gen:"));
		logs.info["res"] << "generating mesh for '" << generateName << "'..." << TGen::endl;
		TGen::Engine::MeshGenerator generator;
		newMesh = generator.generateMesh(generateName, renderer);
	}
	else {
		// TODO: checka filformat för parser
		TGen::Engine::File * file = filesystem.openRead(name);
		TGen::MD3::Parser modelParser;
		TGen::MD3::File * md3File = modelParser.parse(*file);
		delete file;
		
		//md3File->printInfo(std::cout);
		
		newMesh = md3File->createMesh(vertexCache, 0.001);	// TODO: 0.001 är scale factor, en global scale factor och sen kunna sätta per objekt?
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
	
	TGen::Engine::TextPreprocessor processor;
	std::string fixedContents = processor.process(contents, "");
	
	std::list<TGen::Material *> materialsLoaded;
	TGen::MaterialParser parser;
	parser.parse(fixedContents.c_str(), materialsLoaded);
	
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

void TGen::Engine::ResourceManager::updateMaterials(scalar time) {
	MaterialMap::iterator iter = materials.begin();
	for (; iter != materials.end(); ++iter) {
		iter->second->update(time);
	}
}
