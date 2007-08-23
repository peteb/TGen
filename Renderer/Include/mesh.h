/*
 *  mesh.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MESH_H
#define _TGEN_RENDERER_MESH_H

#include <string>

namespace TGen {
	class Mesh {
	public:	
		Mesh(const std::string & name);
		
		std::string getName() const;
		
	private:
		std::string name;
	};	
	
} // !TGen

#endif // !_TGEN_RENDERER_MESH_H

