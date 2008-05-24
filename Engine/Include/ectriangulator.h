/*
 *  ectriangulator.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/24/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_ECTRIANGULATOR_H
#define _TGEN_ENGINE_ECTRIANGULATOR_H

#include <tgen_math.h>

namespace TGen {
	namespace Engine {
		class EarClippingTriangulator {
		public:

			void add(const TGen::Vector3 & point);
			void triangulate(std::vector<unsigned int> & indices);	
			
		private:
			bool insideTriangle(const TGen::Vector3 & A, const TGen::Vector3 & B, const TGen::Vector3 & C, const TGen::Vector3 & P);			
			float area();
			bool snip(int u, int v, int w, int n, int * V); 			
			void process(std::vector<unsigned int> & indices);
			
			std::vector<TGen::Vector3> points;
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_ECTRIANGULATOR_H

