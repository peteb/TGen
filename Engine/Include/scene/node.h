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
		//class EntityList;
		
		namespace Scene {
			class Node : public TGen::Engine::Component, public TGen::Engine::WorldObject {
			public:
				Node(const std::string & name, TGen::SceneNode * sceneNode);
				~Node();
				
				TGen::SceneNode * getSceneNode();
				void update(scalar dt);

				void setLink(const std::string & linkName);
				void setAutoParent(const std::string & autoParent);
				void linkGlobally(TGen::Engine::EntityList & list, TGen::Engine::Entity & entity);
				
				
				// world object interface
				TGen::Vector3 getPosition() const;
				TGen::Rotation getOrientation() const;
				TGen::Vector3 getVelocity() const;
				void setPosition(const TGen::Vector3 & pos);
				void setOrientation(const TGen::Rotation & orientation);			
				
				
			private:
				const TGen::Matrix4x4 & getParentInverseTransform();
				
				TGen::SceneNode * sceneNode;
				TGen::Vector3 velocity, lastPos;
				TGen::Matrix4x4 parentInverseTransform;
				bool changed;
				std::string linkName, autoParent;
			};
		
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENE_NODE_H
