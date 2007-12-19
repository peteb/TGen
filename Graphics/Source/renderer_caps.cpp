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

TGen::RendererCaps::RendererCaps() 
	: maxTextureUnits(0)
	, maxActiveLights(0)
	, maxClipPlanes(0)
	, maxIndexBufferIndicies(0)
	, maxVertexBufferVertices(0)
	, maxTextureSize(0)
	, maxFrameBufferColorAttachments(0)
	, shadingLanguageVersionNumber(0) 
	, vertexShader(false)
	, fragmentShader(false)
	, geometryShader(false)
	, framebuffer(false)
	, multitexturing(false)
	, rectTexture(false)
	, cubeMaps(false)
	, multithreadable(false)
{

}

TGen::RendererCaps::operator std::string() const {
	std::stringstream ss;
	ss << "shading language version: " << shadingLanguageVersion << "\n";
	ss << "driver name: " << driverName << "\n";
	ss << "driver vendor: " << driverVendor << "\n";
	ss << "renderer name: " << rendererName << "\n";
	ss << "max texture units: " << maxTextureUnits << "\n";
	ss << "max texture size: " << maxTextureSize << "\n";
	ss << "max active lights: " << maxActiveLights << "\n";
	ss << "max clip planes: " << maxClipPlanes << "\n";
	ss << "max (recommended) indicies per ib: " << maxIndexBufferIndicies << "\n";
	ss << "max (recommended) vertices per vb: " << maxVertexBufferVertices << "\n";
	ss << "max viewport size: " << std::string(maxViewportSize) << "\n";
	ss << "max framebuffer color attachments: " << maxFrameBufferColorAttachments << "\n";
	ss << "vertex shader: " << std::boolalpha << vertexShader << "\n";
	ss << "fragment shader: " << fragmentShader << "\n";
	ss << "geometry shader: " << geometryShader << "\n";
	ss << "framebuffer: " << framebuffer << "\n";
	ss << "multitexturing: " << multitexturing << "\n";
	ss << "rectangle textures: " << rectTexture << "\n";
	ss << "cube maps: " << cubeMaps << "\n";
	ss << "driver multithreadable: " << multithreadable << "\n";
	
	return ss.str();	
}

