/*
 *  modelgenerator.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MODELGENERATOR_H
#define _TGEN_ENGINE_MODELGENERATOR_H

namespace TGen {
	class VertexDataSource;
	class VertexTransformer;
	class NewModel;
	
	namespace Engine {
		class GenerateLine;
		
		class ModelGenerator {
		public:
			TGen::NewModel * createModel(const TGen::Engine::GenerateLine & line, TGen::VertexDataSource & dataSource, const TGen::VertexTransformer & transformer);
			
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MODELGENERATOR_H

