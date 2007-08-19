/*
 *  meshsource.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// A class that implements the MeshSource interface can be used to link MeshGeometries with their
// mesh data.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_MESHSOURCE_H
#define _TGEN_RENDERER_MESHSOURCE_H

#include <tgen_core.h>

namespace TGen {
	class MeshSource {
	public:
		virtual ~MeshSource() {}
		
		virtual void * getMesh(const std::string & name) abstract;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_MESHSOURCE_H
