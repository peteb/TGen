/*
 *  renderer_types.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderer_types.h"
#include <tgen_core.h>

TGen::PrimitiveType TGen::stringToPrimitive(const std::string & prim) {
	std::string lower = TGen::toLower(prim);
	
	if (lower == "quadstrip")
		return TGen::PrimitiveQuadStrip;
	else if (lower == "quads")
		return TGen::PrimitiveQuads;
	else if (lower == "trianglestrip")
		return TGen::PrimitiveTriangleStrip;
	else if (lower == "trianglefan")
		return TGen::PrimitiveTriangleFan;
	else if (lower == "triangles")
		return TGen::PrimitiveTriangles;
	else if (lower == "points")
		return TGen::PrimitivePoints;
	else if (lower == "linestrip")
		return TGen::PrimitiveLineStrip;
	else if (lower == "lineloop")
		return TGen::PrimitiveLineLoop;
	else if (lower == "lines")
		return TGen::PrimitiveLines;
	else if (lower == "polygon")
		return TGen::PrimitivePolygon;
	
	throw TGen::NotImplemented("stringToPrimitive", "primitive '" + prim + "' not supported");	
}


