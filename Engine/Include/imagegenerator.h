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
	class Canvas;
	
	namespace Engine {
		class GenerateLine;
		
		class ImageGenerator {
		public:
			TGen::Image * generateImage(const TGen::Engine::GenerateLine & line);
			
			
		private:
			enum FadeDirection {
				FadeU,
				FadeV,
			};
			
			void genPlain(const TGen::Engine::GenerateLine & line, TGen::Canvas * canvas);
			void genFade(const TGen::Engine::GenerateLine & line, TGen::Canvas * canvas, FadeDirection direction);
			void genFadeCircle(const TGen::Engine::GenerateLine & line, TGen::Canvas * canvas);
			void genChecker(const TGen::Engine::GenerateLine & line, TGen::Canvas * canvas);
		};
		
	} // !Engine
} // !TGen


#endif // !_TGEN_ENGINE_IMAGEGENERATOR_H
