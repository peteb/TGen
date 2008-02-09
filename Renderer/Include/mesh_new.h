/*
 *  mesh_new.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 2/3/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MESH_NEW_H
#define _TGEN_RENDERER_MESH_NEW_H

#include <tgen_graphics.h>
#include "meshinstance_new.h"

namespace TGen {
	class NewMesh : public TGen::NewMeshInstance {
	public:
		NewMesh(const std::string & materialName, const std::string & materialNamePostfix);
		virtual ~NewMesh();
		
		virtual void update() {}
		virtual TGen::NewMeshInstance * instantiate() const abstract;
	};
	
	
} // !TGen

#endif // !_TGEN_RENDERER_MESH_NEW_H
