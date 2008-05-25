/*
 *  nodetransformer.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/25/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_NODETRANSFORMER_H
#define _TGEN_ENGINE_NODETRANSFORMER_H

#include <tgen_graphics.h>

namespace TGen {
	namespace Engine {
		namespace Scene {
			class TransformNode;
			
			
			class NodeTransformer {
			public:
				NodeTransformer(bool relative);
				virtual ~NodeTransformer() {}
				
				virtual void transform(TGen::Engine::Scene::TransformNode & node, scalar time) abstract;
			
			protected:
				bool relative;
			};
			
			
			
			class NodePositionWaveTransformer : public NodeTransformer {
			public:
				NodePositionWaveTransformer(bool relative, const TGen::Vector3 & axis, std::auto_ptr<TGen::ScalarGenerator> & generator);
				
				void transform(TGen::Engine::Scene::TransformNode & node, scalar time);
				
			private:
				std::auto_ptr<TGen::ScalarGenerator> generator;
				TGen::Vector3 axis;
			};
			
	
			class NodeOrientationWaveTransformer : public NodeTransformer {
			public:
				NodeOrientationWaveTransformer(bool relative, const TGen::Vector3 & axis, std::auto_ptr<TGen::ScalarGenerator> & generator);
				
				void transform(TGen::Engine::Scene::TransformNode & node, scalar time);
				
			private:
				std::auto_ptr<TGen::ScalarGenerator> generator;
				TGen::Vector3 axis;
			};
			
			
			
		} // !Scene
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_NODETRANSFORMER_H

