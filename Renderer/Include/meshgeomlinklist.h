/*
 *  meshgeomlinklist.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MESHGEOMLINKLIST_H
#define _TGEN_RENDERER_MESHGEOMLINKLIST_H

#include "meshgeometry.h"

namespace TGen {
	class MeshSource;
	
	class MeshGeometryLinkList : public TGen::MeshGeometryObserver {
	public:	
		MeshGeometryLinkList();
		~MeshGeometryLinkList();
		
		TGen::Geometry * attach(TGen::MeshGeometry * geometry);
		void onRemoved(TGen::MeshGeometry & geometry);
		void relink(TGen::MeshSource & source);
		
	private:
		typedef std::vector<TGen::MeshGeometry *> GeometryList;
		GeometryList attached;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_MESHGEOMLINKLIST_H
