/*
 *  renderer_caps.h
 *  tgen
 *
 *  Created by Peter Backman on 6/19/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_CAPS_H
#define _TGEN_RENDERER_CAPS_H

#include <string>
#include "rectangle.h"

namespace TGen {
	class Renderer;
	
	class RendererCaps {
	private:
		RendererCaps();
		
	public:
		int maxTextureUnits, maxActiveLights, maxClipPlanes, maxIndexBufferIndicies, 
			maxVertexBufferVertices, maxTextureSize, maxFrameBufferColorAttachments;
		int shadingLanguageVersionNumber;
		std::string shadingLanguageVersion, rendererName, driverName, driverVendor;
		bool vertexShader, fragmentShader, geometryShader, framebuffer, multitexturing, rectTexture, cubeMaps, multithreadable;
		
		TGen::Rectangle maxViewportSize;
		
		operator std::string() const;
		
		friend class Renderer;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_CAPS_H
