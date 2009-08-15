/*
 *  vertexoffsetter.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 5/6/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_VERTEXOFFSETTER_H
#define _TGEN_RENDERER_VERTEXOFFSETTER_H

#include "vertextransformer.h"
#include <tgen_math.h>

namespace TGen {
	class VertexOffsetter : public TGen::VertexTransformer {
	public:
		VertexOffsetter(const TGen::Vector3 & offset);
		
		void transform(TGen::Vector3 & vector) const;
		void transform(TGen::Plane3 & Plane3) const;
		TGen::VertexOffsetter * clone() const;
		
	private:
		TGen::Vector3 offset;
	};
} // !TGen

#endif // !_TGEN_RENDERER_VERTEXOFFSETTER_H

