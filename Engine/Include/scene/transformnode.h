/*
 *  transformnode.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_SCENE_TRANSFORMNODE_H
#define _TGEN_ENGINE_SCENE_TRANSFORMNODE_H

#include <tgen_renderer.h>

namespace TGen {
	namespace Engine {
		namespace Scene {
			class NodeTransformer;
			
			class TransformNode : public TGen::SceneNode {
			public:	
				TransformNode(const std::string & name);
				TransformNode(const TransformNode & node);
				~TransformNode();
				
				void addPositionTransformer(const TGen::PropertyTree & properties);
				void addOrientationTransformer(const TGen::PropertyTree & properties);
				
				virtual void update(scalar dt);
				TransformNode * clone();
				
			private:
				TGen::WaveGenerator * createWaveGenerator(const TGen::PropertyTree & properties) const;
				
				std::vector<TGen::Engine::Scene::NodeTransformer *> transformers;
				scalar age;
			};
			
			class DummyTransformNode : public TransformNode {
			public:
				DummyTransformNode(const std::string & name) : TransformNode(name) {}
				
				void update(scalar dt) {}
			};
			
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_SCENE_TRANSFORMNODE_H

