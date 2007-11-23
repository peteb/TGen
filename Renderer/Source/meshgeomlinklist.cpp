/*
 *  meshgeomlinklist.cpp
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/23/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "meshgeomlinklist.h"
#include "meshsource.h"
#include <algorithm>

TGen::MeshGeometryLinkList::MeshGeometryLinkList() {
	
}

TGen::MeshGeometryLinkList::~MeshGeometryLinkList() {
	for (int i = 0; i < attached.size(); ++i)
		attached[i]->detachedFromObserver(this);
}

TGen::Geometry * TGen::MeshGeometryLinkList::attach(TGen::MeshGeometry * geometry) {
	attached.push_back(geometry);
	geometry->attachedToObserver(this);
	
	return geometry;
}

void TGen::MeshGeometryLinkList::onRemoved(TGen::MeshGeometry & geometry) {
	GeometryList::iterator iter = std::find(attached.begin(), attached.end(), &geometry);
	if (iter != attached.end())
		attached.erase(iter);	
}

void TGen::MeshGeometryLinkList::relink(TGen::MeshSource & source) {
	for (int i = 0; i < attached.size(); ++i) {
		attached[i]->linkMesh(source);
		
	}
}
