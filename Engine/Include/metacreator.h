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
	class Vector3;
	class Color;
	
	namespace Engine {
		class MetaCreator {
		public:
			void writeAxes(const TGen::Matrix4x4 & coordsys, TGen::VertexStream & stream);	
			void writePolygon(const TGen::Matrix4x4 & transform, TGen::VertexStream & stream, const TGen::Vector3 * coords, int numCoords, const TGen::Color & color);
			
		};
		
	} // !Engine
} // !TGen

#endif // !_TGEN_ENGINE_METACREATOR_H
