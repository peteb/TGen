/*
 *  vertexswapper.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 3/26/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_VERTEXSWAPPER_H
#define _TGEN_RENDERER_VERTEXSWAPPER_H

#include "vertextransformer.h"

namespace TGen {
	class VertexSwapper : public TGen::VertexTransformer {
	public:
		enum SwapAxes {
			X_AXIS = 1,
			Y_AXIS = 2,
			Z_AXIS = 3,
		};
		
		VertexSwapper(const SwapAxes & ax1, const SwapAxes & ax2);
		
		void transform(TGen::Vector3 & vector) const;
		void transform(TGen::Plane3 & plane) const;

		TGen::VertexSwapper * clone() const;
		

	private:
		SwapAxes ax1, ax2;
	};
	
} // !TGen

#endif // !_TGEN_RENDERER_VERTEXSWAPPER_H

