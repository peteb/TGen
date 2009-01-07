/*
 *  vertexdata.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 1/7/09.
 *  Copyright 2009 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_VERTEXDATA_H
#define _TGEN_GRAPHICS_VERTEXDATA_H

#include <tgen_core.h>

namespace TGen {
	class VertexDataSource;
	class ShaderVariable;
	class VertexStructure;
	
	class VertexData {
	protected:
		VertexData(TGen::VertexDataSource & creator, uint size, ushort usage, void * writeOffset = NULL, uint readOffset = 0);
		
	public:
		virtual ~VertexData();
		
		virtual void * lock(ushort flags) abstract;
		virtual void unlock() abstract;
		virtual void bufferData(const void * data, uint size, void * offset) abstract;
		virtual void bindShaderVariable(int id, const TGen::ShaderVariable & var) abstract;
		virtual TGen::VertexStructure & getVertexStructure() abstract;
		virtual TGen::VertexData * operator * ();
		
		uint getSortKey() const;
		uint getSize() const;
		uint getUsage() const;
		uint getReadOffset() const;
		
	protected:
		uint size, usage, readOffset;
		void * writeOffset;
		TGen::VertexDataSource & creator;
	};	
} // !TGen

#endif // !_TGEN_GRAPHICS_VERTEXDATA_H
