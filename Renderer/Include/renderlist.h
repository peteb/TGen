/*
 *  renderlist.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/14/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_RENDERLIST_H
#define _TGEN_RENDERER_RENDERLIST_H

#include <vector>
#include <tgen_core.h>
#include <tgen_math.h>

namespace TGen {
	class Face;
	class Camera;
	class Renderer;
	
	class RenderList {
	public:
		RenderList() {}
		virtual ~RenderList() {}
		
		virtual void render(TGen::Renderer & renderer, const TGen::Camera & camera) abstract;
		virtual void addFace(const TGen::Face * face) abstract;
		virtual void sort(const TGen::Camera & camera) abstract;
		virtual void clear() abstract;
		virtual bool needSorting() abstract;
	};
} // !TGen

#endif // !_TGEN_RENDERER_RENDERLIST_H
