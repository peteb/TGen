/*
 *  renderlist.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/18/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_RENDERLIST_H
#define _THEQUAKER_RENDERLIST_H

#include <tgen_graphics.h>
#include "surface.h"

class Camera;

class RenderList {
public:	
	RenderList();
	~RenderList();
	
	void Render(TGen::Renderer & renderer, const Camera & camera);
	void AddSurface(const Surface * surface);
	void Clear();
	
private:
	typedef std::vector<const Surface *> SurfaceList; 
	
	SurfaceList surfaces;
};

#endif // !_THEQUAKER_RENDERLIST_H