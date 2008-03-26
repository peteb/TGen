/*
 *  vertextransformlist.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_VERTEXTRANSFORMLIST_H
#define _TGEN_RENDERER_VERTEXTRANSFORMLIST_H

#include "vertextransformer.h"
#include <vector>

namespace TGen {
	class VertexTransformList : public TGen::VertexTransformer {
	public:
		VertexTransformList();
		~VertexTransformList();
		
		void transform(TGen::Vector3 & vector) const;
		void addTransformer(TGen::VertexTransformer * transformer);
		void addTransformerFirst(TGen::VertexTransformer * transformer);
		
		TGen::VertexTransformList * clone() const;
		
	private:
		std::vector<TGen::VertexTransformer *> transformers;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_VERTEXTRANSFORMLIST_H

