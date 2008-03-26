/*
 *  vertexscaler.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_VERTEXSCALER_H
#define _TGEN_RENDERER_VERTEXSCALER_H

#include "vertextransformer.h"

namespace TGen {
	class VertexScaler : public TGen::VertexTransformer {
	public:
		VertexScaler(scalar scale);
		
		void transform(TGen::Vector3 & vector) const;
		TGen::VertexScaler * clone() const;
		
	private:
		scalar scale;
	};
} // !TGen

#endif // !_TGEN_RENDERER_VERTEXSCALER_H

