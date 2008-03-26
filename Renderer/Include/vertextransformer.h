/*
 *  vertextransformer.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_VERTEXTRANSFORMER_H
#define _TGEN_RENDERER_VERTEXTRANSFORMER_H

#include <tgen_core.h>

namespace TGen {
	class Vector3;
	
	class VertexTransformer {
	public:
		virtual ~VertexTransformer() {}
		
		virtual void transform(TGen::Vector3 & vector) const abstract;
		virtual TGen::VertexTransformer * clone() const abstract;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_VERTEXTRANSFORMER_H

