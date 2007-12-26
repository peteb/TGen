/*
 *  vertexsource.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 12/25/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_VERTEXSOURCE_H
#define _TGEN_VERTEXSOURCE_H

#include <tgen_core.h>

namespace TGen {
	class VertexStructure;
	class VertexDataSource;
	class ShaderVariable;
	
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
	
	
	class VertexDataSource {
	public:
		virtual ~VertexDataSource() {}
		
		virtual VertexData * createVertexData(const VertexStructure & vertstruct, uint size, ushort usage) abstract;
		
		friend class VertexData;
		
	private:
		virtual void removeVertexData(VertexData * data) abstract;
		
	};
	
	
}

#endif // !_TGEN_VERTEXSOURCE_H
