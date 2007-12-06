/*
 *  meshgenerator.cpp
 *  TGen Engine
 *
 *  Created by Peter Backman on 11/11/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "meshgenerator.h"
#include "fillquad.h"
#include <iostream>

TGen::Mesh * TGen::Engine::MeshGenerator::generateMesh(const std::string & name, TGen::Renderer & renderer) {
	if (name == "fillquad") {
		return new TGen::Engine::FillQuad(TGen::Vector2(-1.0f, -1.0f), TGen::Vector2(1.0f, 1.0f), renderer);
	}
	
	return NULL;
}
