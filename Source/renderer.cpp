/*
 *  renderer.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/12/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderer.h"

TGen::Renderer::Renderer() : clearColor(TGen::Color::Black) {}
TGen::Renderer::~Renderer() {}

void TGen::Renderer::setClearColor(const TGen::Color & color) {
	clearColor = color;
}

TGen::Color TGen::Renderer::getClearColor() const {
	return clearColor;
}
