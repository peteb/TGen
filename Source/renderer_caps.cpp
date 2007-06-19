/*
 *  renderer_caps.cpp
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#include "renderer_caps.h"
#include <sstream>

TGen::RendererCaps::RendererCaps() : maxTextureUnits(0), maxActiveLights(0), maxClipPlanes(0), maxIndexBufferIndicies(0), maxVertexBufferVertices(0), maxTextureSize(0) {}

TGen::RendererCaps::operator std::string() const {
	std::stringstream ss;
	ss << "max texture units: " << maxTextureUnits << "\n";
	ss << "max texture size: " << maxTextureSize << "\n";
	ss << "max active lights: " << maxActiveLights << "\n";
	ss << "max clip planes: " << maxClipPlanes << "\n";
	ss << "max (recommended) indicies per ib: " << maxIndexBufferIndicies << "\n";
	ss << "max (recommended) vertices per vb: " << maxVertexBufferVertices << "\n";
	ss << "max viewport size: " << std::string(maxViewportSize) << "\n";
	
	return ss.str();	
}




