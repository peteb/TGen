/*
 *  md5submesh.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 1/23/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD5MESH_H
#define _TGEN_RENDERER_MD5MESH_H

#include <tgen_renderer.h>

namespace TGen {
	namespace MD5 {
		class Mesh {
		public:
			Mesh(const std::string & materialName);
			~Mesh();
			
			std::string getMaterialName() const;
			
		private:
			std::string materialName;
		};
		
	} // !MD5 
} // !TGen

#endif // !_TGEN_RENDERER_MD5MESH_H
