/*
 *  resourcemanager.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_RESOURCEMANAGER_H
#define _TGEN_ENGINE_RESOURCEMANAGER_H

#include <tgen_graphics.h>
#include <tgen_renderer.h>
#include <string>
#include "imageloader.h"
#include "vertexcache.h"
#include "preprocessor.h"

namespace TGen {
	namespace Engine {
		class Filesystem;
		class StandardLogs;
		class Renderer;
		class VariableRegister;

		
		enum ShaderModeAttributes {
			LIGHT_AMBIENT =		0x0001,
			LIGHT_SPOT =			0x0002,
			LIGHT_OMNI =			0x0004,
			LIGHT_FILTER =			0x0008,

			SHADOWMAP =				0x0010,
		};
		
		class ResourceManager : public TGen::MaterialLinkCallback, public TGen::MaterialSource, public TGen::MeshSource, public TGen::ModelInstantiator, public TGen::Engine::IncludeCallback {
		public:
			ResourceManager(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem, TGen::Renderer & renderer, TGen::Engine::VariableRegister & variables);
			~ResourceManager();
			
			TGen::ShaderProgram * getShaderProgram(const std::string & name);
			TGen::Texture * getTexture(const std::string & name);
			TGen::Mesh * getMesh(const std::string & name);
			TGen::NewModelInstance * instantiateModel(const std::string & name);
			TGen::Material * getMaterial(const std::string & name);
			int getTextureType(const std::string & name);
			void loadMaterials(const std::string & filename);
			void updateMaterials(scalar time);
			
			std::string getIncludeContent(const std::string & identifier);
			
			TGen::Engine::VertexCache vertexCache;
			
		private:
			typedef std::map<std::string, TGen::ShaderProgram *> ShaderMap;
			typedef std::map<std::string, TGen::Texture *> TextureMap;
			typedef std::map<std::string, TGen::Mesh *> MeshMap;
			typedef std::map<std::string, TGen::Material *> MaterialMap;
			typedef std::map<std::string, TGen::NewModel *> ModelMap;
			
			ShaderMap shaders;
			TextureMap textures;
			MeshMap meshes;
			MaterialMap materials;
			ModelMap models;
			
			TGen::Engine::ImageLoader imageLoader;
			TGen::Engine::Filesystem & filesystem;
			TGen::Engine::StandardLogs & logs;
			TGen::Engine::VariableRegister & variables;
			TGen::Renderer & renderer;
		};
	}
}

#endif // !_TGEN_ENGINE_RESOURCEMANAGER_H
