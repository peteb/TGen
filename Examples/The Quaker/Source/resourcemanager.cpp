/*
 *  resourcemanager.cpp
 *  The Quaker
 *
 *  Created by Peter Backman on 7/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "resourcemanager.h"
#include <iostream>
#include <IL/IL.h>
#include "devil.h"

ResourceManager::ResourceManager(TGen::Renderer & renderer) : renderer(renderer) {
	std::cout << "[res]: initializing..." << std::endl;
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
}

ResourceManager::~ResourceManager() {
	std::cout << "[res]: shutting down..." << std::endl;
	
	for (ShaderMap::iterator iter = shaders.begin(); iter != shaders.end(); ++iter)
		delete iter->second;
	
	for (TextureMap::iterator iter = textures.begin(); iter != textures.end(); ++iter)
		delete iter->second;
	
	for (MaterialMap::iterator iter = materials.begin(); iter != materials.end(); ++iter)
		delete iter->second;	
}

TGen::ShaderProgram * ResourceManager::getShaderProgram(const std::string & name) {
	ShaderMap::iterator iter = shaders.find(name);
	if (iter != shaders.end())
		return iter->second;
	
	std::cout << "[res]: loading shader '" << name << "'..." << std::endl;
	std::string data = ReadFile(name);
	
	TGen::ShaderProgram * ret = renderer.createShaderProgram(data.c_str());
	ret->link();
	
	shaders[name] = ret;
	
	return ret;	
} // RGBGEN!!!!! 

TGen::Texture * ResourceManager::getTexture(const std::string & name) {
	if (name == "$lightmap")
		return NULL;

	if (name.empty())
		return NULL;
	
	TextureMap::iterator iter = textures.find(name);
	if (iter != textures.end())
		return iter->second;
	
	std::cout << "[res]: loading texture '" + name + "'..." << std::endl;
	
	ILuint imageName;
	ilGenImages(1, &imageName);
	ilBindImage(imageName);
	
	if (!ilLoadImage(name.c_str()))
		throw TGen::RuntimeException("ResourceManager::getTexture", "image failed to load image with DevIL");
	
	TGen::Texture * newTexture = renderer.createTexture(DevilImage(imageName), TGen::RGBA);
	ilDeleteImages(1, &imageName);
	
	textures[name] = newTexture;
	
	return newTexture;
}

TGen::Material * ResourceManager::getMaterial(const std::string & name) {
	MaterialMap::iterator iter = materials.find(name);
	if (iter != materials.end())
		return iter->second;
	
	return materials["noshader"];
	
	throw TGen::RuntimeException("ResourceManager::getMaterial", "material '" + name + "' not loaded!");
}

int ResourceManager::getTextureType(const std::string & name) {
	if (name == "$lightmap")
		return 1;

	return 0;
}

std::string ResourceManager::ReadFile(const std::string & filename) const {
	std::string ret;
	
	FILE * file = fopen(filename.c_str(), "r");
	if (!file)
		throw TGen::RuntimeException("ResourceManager::ReadFile", "file '" + filename + "' not found");
	
	char buffer[1024];
	while (!feof(file)) {
		int read = fread(buffer, 1, 1022, file);
		buffer[read] = 0;
		ret += buffer;
	}
	
	fclose(file);
	
	return ret;	
}

void ResourceManager::LoadMaterials(const std::string & filename) {
	std::cout << "[res]: loading materials from '" + filename + "'..." << std::endl;
	
	std::list<TGen::Material *> materials;
	TGen::Q3MaterialParser parser;
	parser.parse(ReadFile(filename).c_str(), materials);
	
	for (std::list<TGen::Material *>::iterator iter = materials.begin(); iter != materials.end(); ++iter) {
		(*iter)->link(*this);
		this->materials[(*iter)->getName()] = *iter;
	}	
	
	std::cout << "[res]: created materials: ";
	int i = 0;
	
	for (std::list<TGen::Material *>::iterator iter = materials.begin(); iter != materials.end(); ++iter) {
		std::cout << (*iter)->getName() << " ";
		if (i > 4) {
			std::cout << std::endl;
			i = 0;
		}		
	}
	
	std::cout << std::endl;
}


void ResourceManager::UpdateMaterials(scalar time) {
	for (MaterialMap::iterator iter = materials.begin(); iter != materials.end(); ++iter)
		iter->second->update(time);
}
