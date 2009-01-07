/*
 *  vertexdata_ogl.h
 *  TGen OpenGL
 *
 *  Created by Peter Backman on 12/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_OPENGL_VERTEXDATA_H
#define _TGEN_OPENGL_VERTEXDATA_H

#include "vertexsource.h"
#include "vertexdata.h"
#include "vertexstructure.h"
#include "prefix_ogl.h"

namespace TGen {	
	class RenderStatistics;

	namespace OpenGL {
		class Renderer;
		
		class VertexData : public TGen::VertexData {
		private:
			VertexData(TGen::VertexDataSource & creator, 
						  TGen::RenderStatistics & stats, 
						  const TGen::VertexStructure & vertstruct, 
						  uint size, ushort usage, GLuint vboId);
			
		public:
			~VertexData();
			
			void * lock(ushort flags);
			void unlock();
			void bufferData(const void * data, uint size, void * offset);
			bool isLocked();
			void bindShaderVariable(int id, const TGen::ShaderVariable & var);
			
			TGen::VertexStructure & getVertexStructure();
			
			GLuint getInternalID() const;
			
			friend class TGen::OpenGL::Renderer;
			
		private:
			TGen::VertexStructure vertstruct;
			TGen::RenderStatistics & stats;
			GLuint vboId;
		};
		
	} // !OpenGL
} // !TGen

#endif // !_TGEN_OPENGL_VERTEXDATA_H

