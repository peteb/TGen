/*
 *  node.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/5/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCENE_NODE_H
#define _TGEN_ENGINE_SCENE_NODE_H

#include "component.h"
#include "componentinterfaces.h"

namespace TGen {
	class SceneNode;
	
	namespace Engine {
		namespace Scene {
			class Node : public TGen::Engine::Component, public TGen::Engine::ObjectInterface {
			public:
				Node(const std::string & name, TGen::SceneNode * sceneNode);
				~Node();
				
				TGen::SceneNode * getSceneNode();
				void update(scalar dt);

				// object interface
				TGen::Vector3 getPosition() const;
				TGen::Rotation getOrientation() const;
				TGen::Vector3 getVelocity() const;
				TGen::Matrix4x4 getSpaceTransform() const;
				void setPosition(const TGen::Vector3 & pos);
				void setOrientation(const TGen::Rotation & orientation);			
				
				
			private:
				TGen::SceneNode * sceneNode;
				TGen::Vector3 velocity, lastPos;
			};
		
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENE_NODE_H
