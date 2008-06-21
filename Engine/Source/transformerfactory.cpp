/*
 *  transformerfactory.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "transformerfactory.h"
#include "generateline.h"
#include <tgen_renderer.h>

TGen::VertexTransformList * TGen::Engine::TransformerFactory::createTransformers(const TGen::Engine::GenerateLine & line) const {
	std::auto_ptr<TGen::VertexTransformList> transformers(new TGen::VertexTransformList);
	
	for (TGen::Engine::GenerateLine::ParameterMap::const_iterator iter = line.getParameters().begin(); iter != line.getParameters().end(); ++iter) {
		if (iter->first == "scale") {
			transformers->addTransformer(new TGen::VertexScaler(TGen::lexical_cast<scalar>(iter->second)));
		}
		else if (iter->first == "preset") {
			if (iter->second == "q3" || iter->second == "idtech") {
				transformers->addTransformer(new TGen::VertexSwapper(TGen::VertexSwapper::Y_AXIS, TGen::VertexSwapper::Z_AXIS));
				transformers->addTransformer(new TGen::VertexScaler(0.0254));		// 0.038 
				transformers->addTransformer(new TGen::VertexMatrixTransformer(TGen::Matrix4x4::RotationY(TGen::Degree(90))));
			}
			else {
				throw TGen::RuntimeException("TransformerFactory::createTransformers", "invalid component transform preset: '" + iter->second + "'");
			}
		}
		else if (iter->first == "rotY") {
			TGen::Matrix4x4 transform = TGen::Matrix4x4::RotationY(TGen::Degree(TGen::lexical_cast<scalar>(iter->second)));
			transformers->addTransformer(new TGen::VertexMatrixTransformer(transform));
		}
	}
	
	return transformers.release();
}
