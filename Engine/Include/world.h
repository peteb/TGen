/*
 *  world.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/16/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_WORLD_H
#define _TGEN_ENGINE_WORLD_H

#include <string>
#include <tgen_renderer.h>
#include "lightlist.h"

namespace TGen {
	class Camera;
	
	namespace Engine {
		class App;
		
		class World {
		public:
			World(TGen::Engine::App & app);
			~World();
			
			TGen::Camera * getCamera(const std::string & name);
			TGen::RenderList & getRenderList();
			TGen::Engine::LightList & getLightList();
			TGen::Color getAmbientLight();
			
			void prepareLists(TGen::Camera * camera);
			
			void update(scalar dt);
			
		private:
			TGen::Engine::App & app;
			TGen::SceneNode sceneRoot;
			TGen::BasicRenderList renderList;
			TGen::Engine::LightList lightList;
			
			TGen::Engine::Light * lights[4];
			
			TGen::Camera * mainCam;
		};
		
	} // Engine
} // !TGen

#endif // !_TGEN_ENGINE_WORLD_H
