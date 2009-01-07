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
	class VertexData;
	
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
