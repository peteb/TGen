/*
 *  metacreator.h
 *  TGen Engine
 *
 *  Created by Peter Backman on 2/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_ENGINE_METACREATOR_H
#define _TGEN_ENGINE_METACREATOR_H

namespace TGen {
	class VertexStream;
	class Matrix4x4;
	
	namespace Engine {
		class MetaCreator {
		public:
			void writeAxes(const TGen::Matrix4x4 & coordsys, TGen::VertexStream & stream);			
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_METACREATOR_H
