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

namespace TGen {
	namespace Engine {
		class Filesystem;
		class StandardLogs;
		class Renderer;
		
		class ResourceManager : public TGen::MaterialLinkCallback {
		public:
			ResourceManager(TGen::Engine::StandardLogs & logs, TGen::Engine::Filesystem & filesystem, TGen::Renderer & renderer);
			~ResourceManager();
			
			TGen::ShaderProgram * getShaderProgram(const std::string & name);
			TGen::Texture * getTexture(const std::string & name);
			TGen::Mesh * getMesh(const std::string & name);
			int getTextureType(const std::string & name);
			
		private:
			typedef std::map<std::string, TGen::ShaderProgram *> ShaderMap;
			typedef std::map<std::string, TGen::Texture *> TextureMap;
			typedef std::map<std::string, TGen::Mesh *> MeshMap;
			
			ShaderMap shaders;
			TextureMap textures;
			MeshMap meshes;
			
			TGen::Engine::Filesystem & filesystem;
			TGen::Engine::StandardLogs & logs;
			TGen::Renderer & renderer;
		};
	}
}

#endif // !_TGEN_ENGINE_RESOURCEMANAGER_H
