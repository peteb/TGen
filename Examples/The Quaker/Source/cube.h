/*
 *  cube.h
 *  The Quaker
 *
 *  Created by Peter Backman on 7/20/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

#ifndef _THEQUAKER_CUBE_H
#define _THEQUAKER_CUBE_H

#include "surface.h"
#include <tgen_graphics.h>

class Cube : public Geometry {
public:	
	Cube(TGen::Renderer & renderer);
	~Cube();
	
	void prepareRender(TGen::Renderer & renderer) const;
	void render(TGen::Renderer & renderer) const;
	TGen::Vector3 getMax() const;
	TGen::Vector3 getMin() const;

private:
	typedef TGen::Index<unsigned short> MyIndex;
	typedef TGen::JoinVertex2<TGen::Vertex3<float>, TGen::TexCoord2<float, 0> > MyVertex;

	TGen::VertexBuffer * vb;
	TGen::IndexBuffer * ib;

	scalar width, height, depth;
};

#endif // !_THEQUAKER_CUBE_H
