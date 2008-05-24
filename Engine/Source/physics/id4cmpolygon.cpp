/*
 *  id4cmpolygon.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 5/17/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#include "physics/id4cmpolygon.h"

TGen::Engine::Physics::Id4CMPolygon::Id4CMPolygon() {
	edges.reserve(4);
}

void TGen::Engine::Physics::Id4CMPolygon::addEdge(int num) {
	edges.push_back(num);
}

int TGen::Engine::Physics::Id4CMPolygon::getNumEdges() const {
	return edges.size();
}

int TGen::Engine::Physics::Id4CMPolygon::getEdge(int num) const {
	return edges[num];
}

