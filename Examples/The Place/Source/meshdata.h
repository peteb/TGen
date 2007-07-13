/*
 *  meshdata.h
 *  The Place
 *
 *  Created by Peter Backman on 7/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_THEPLACE_MESHDATA_H
#define _TGEN_THEPLACE_MESHDATA_H

#include <tgen_graphics.h>
#include <tgen_core.h>

class MeshData {
public:
	MeshData(const TGen::VertexStructure & structure, uint count = 0, void * data = NULL);
	
	uint count;
	const TGen::VertexStructure & structure;
	void * data;
};


#endif // !_TGEN_THEPLACE_MESHDATA_H
