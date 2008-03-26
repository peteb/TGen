/*
 *  modelfactory.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MODELFACTORY_H
#define _TGEN_ENGINE_MODELFACTORY_H

namespace TGen {
	class VertexDataSource;
	class NewModel;
	
	namespace Engine {
		class GenerateLine;
		class Filesystem;
		
		class ModelFactory {
		public:
			ModelFactory(TGen::Engine::Filesystem & filesystem);
			
			TGen::NewModel * createModel(const TGen::Engine::GenerateLine & line, TGen::VertexDataSource & dataSource);
			
		private:
			TGen::Engine::Filesystem & filesystem;
		};
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MODELFACTORY_H

