/*
 *  md3staticmesh.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/26/07.
 *  Renamed by Peter Backman on 2/3/08.
 *  Renamed by Peter Backman on 2/7/08.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MD3STATICMESH_H
#define _TGEN_RENDERER_MD3STATICMESH_H

#include "geometry.h"
#include "mesh_new.h"

namespace TGen {
	class Renderer;
	class VertexDataSource;
	
	namespace MD3 {
		class File;
		
		class StaticMesh : public TGen::NewMesh {
		public:
			StaticMesh(const std::string & materialName, const std::string & materialNamePostfix);
			~StaticMesh();
			
			void preRender(TGen::Renderer & renderer) const;
			void render(TGen::Renderer & renderer) const;

			TGen::MD3::StaticMesh * instantiate(TGen::VertexDataSource & source) const;
			void createVertexData(TGen::VertexDataSource & dataSource);
			
			friend class TGen::MD3::File;
			
		private:
			TGen::VertexData * vb;
			TGen::VertexData * ib;
			uint startIndex, numIndices;
		};
		
		
		
	} // !MD3
} // !TGen

#endif // !_TGEN_RENDERER_MD3STATICMESH_H

