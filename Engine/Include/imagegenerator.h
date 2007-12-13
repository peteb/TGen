/*
 *  imagegenerator.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 12/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_IMAGEGENERATOR_H
#define _TGEN_ENGINE_IMAGEGENERATOR_H

#include <string>

namespace TGen {
	class Image;
	
	namespace Engine {
		class GenerateLine;
		
		class ImageGenerator {
		public:
			TGen::Image * generateImage(const TGen::Engine::GenerateLine & line);
			
		};
		
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_IMAGEGENERATOR_H
