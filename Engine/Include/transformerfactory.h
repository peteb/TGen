/*
 *  transformerfactory.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/27/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_TRANSFORMERFACTORY_H
#define _TGEN_ENGINE_TRANSFORMERFACTORY_H

namespace TGen {
	class VertexTransformList;
	
	namespace Engine {
		class GenerateLine;
		
		class TransformerFactory {
		public:
			
			TGen::VertexTransformList * createTransformers(const TGen::Engine::GenerateLine & line) const;
			
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_TRANSFORMERFACTORY_H

