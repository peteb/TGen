/*
 *  vertexmattransformer.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 6/16/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_VERTEXMATTRANSFORMER_H
#define _TGEN_RENDERER_VERTEXMATTRANSFORMER_H

#include "vertextransformer.h"
#include <tgen_math.h>

namespace TGen {
	class VertexMatrixTransformer : public TGen::VertexTransformer {
	public:
		VertexMatrixTransformer(const TGen::Matrix4x4 & matrix);
		
		void transform(TGen::Vector3 & vector) const;
		TGen::VertexMatrixTransformer * clone() const;
	
	private:
		TGen::Matrix4x4 matrix;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_VERTEXMATTRANSFORMER_H

