/*
 *  meshgenerator.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_MESHGENERATOR_H
#define _TGEN_ENGINE_MESHGENERATOR_H

#include <string>

namespace TGen {
	class Mesh;
	
	namespace Engine {
		class MeshGenerator {
		public:
			TGen::Mesh * generateMesh(const std::string & name);
			
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_MESHGENERATOR_H
