/*
 *  meshsource.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MESHSOURCE_H
#define _TGEN_RENDERER_MESHSOURCE_H

namespace TGen {
	class MeshSource {
	public:
		virtual ~MeshSource() {}
		
		virtual void * getMesh(const std::string & name) abstract;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_MESHSOURCE_H
