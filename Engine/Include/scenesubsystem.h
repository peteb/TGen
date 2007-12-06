/*
 *  scenesubsystem.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCENESUBSYSTEM_H
#define _TGEN_ENGINE_SCENESUBSYSTEM_H

#include "subsystem.h"
#include <tgen_renderer.h>

namespace TGen {
	namespace Engine {
		class Entity;
		class World;
		
		class SceneSubsystem : public TGen::Engine::Subsystem {
		public:
			SceneSubsystem(TGen::Engine::World & world);
			~SceneSubsystem();
			
			TGen::Engine::Component * createComponent(TGen::Engine::Entity * entity, const TGen::PropertyTree & properties);
			void link();
			
			TGen::SceneNode & getSceneRoot();
			
		private:
			TGen::Engine::World & world;
			TGen::SceneNode sceneRoot;
			TGen::MeshGeometryLinkList meshList;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENESUBSYSTEM_H