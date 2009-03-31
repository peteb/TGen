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
#include "worldobject.h"
#include "componentlink.h"

namespace TGen {
	class SceneNode;
	
	namespace Engine {
		//class EntityList;
		class Entity;
		
		namespace Scene {
			class SceneScript;
			
			class Node : public TGen::Engine::Component, public TGen::Engine::WorldObject {
			public:
				Node(const std::string & name, TGen::SceneNode * sceneNode, TGen::Engine::Entity & entity);
				~Node();
				
				TGen::SceneNode * getSceneNode();
				void update(scalar dt);

				void setLink(const std::string & linkName);
				void setAttachComponent(const std::string & componentName);
				void setAttachJoint(const std::string & jointName);
				void setAutoParent(const std::string & autoParent);
				void link(const TGen::Engine::ComponentLinker & linker);
				void setEnabled(bool enabled);
				
				
				// world object interface
				TGen::Vector3 getPosition() const;
				TGen::Rotation getOrientation() const;
				TGen::Vector3 getVelocity() const;
				void setPosition(const TGen::Vector3 & pos);
				void setOrientation(const TGen::Rotation & orientation);			
				
				void setScriptInterface(TGen::Engine::Scene::SceneScript * scriptInterface);
				
			private:
				const TGen::Matrix4x4 & getParentInverseTransform();
				TGen::Engine::Scene::SceneScript * scriptInterface;
				
				TGen::Engine::UnaryDelegate<TGen::Engine::Scene::Node> attachComponent;
				
				TGen::Vector3 bo;
				
				TGen::SceneNode * sceneNode;
				TGen::Vector3 velocity, lastPos;
				TGen::Matrix4x4 parentInverseTransform;
				bool changed;
				std::string linkName, autoParent, attachJoint;
			};
		
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENE_NODE_H
