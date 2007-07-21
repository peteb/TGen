/*
 *  resourcemanager.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_RESOURCEMANAGER_H
#define _THEQUAKER_RESOURCEMANAGER_H

#include <tgen_graphics.h>
#include <map>
#include "surface.h"

class ResourceManager : public TGen::MaterialLinkCallback, public SurfaceLinker {
public:
	ResourceManager(TGen::Renderer & renderer);
	~ResourceManager();
	
	TGen::ShaderProgram * getShaderProgram(const std::string & name);
	TGen::Texture * getTexture(const std::string & name);
	int getTextureType(const std::string & name);
	TGen::Material * getMaterial(const std::string & name);
	void LoadMaterials(const std::string & filename);
	void UpdateMaterials(scalar time);
	
private:	
	std::string ReadFile(const std::string & filename) const;

	typedef std::map<std::string, TGen::ShaderProgram *> ShaderMap;
	typedef std::map<std::string, TGen::Texture *> TextureMap;
	typedef std::map<std::string, TGen::Material *> MaterialMap;
	
	ShaderMap shaders;
	TextureMap textures;
	MaterialMap materials;
	
	TGen::Renderer & renderer;	
};

#endif // !_THEQUAKER_RESOURCEMANAGER_H
