/*
 *  modelrenderproperties.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 7/5/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_MODELRENDERPROPERTIES_H
#define _TGEN_RENDERER_MODELRENDERPROPERTIES_H

#include <tgen_graphics.h>

namespace TGen {
	class ModelRenderProperties {
	public:
		ModelRenderProperties()
			: frontFaceDef(TGen::FaceWindingCCW) {}
		
		TGen::FaceWinding frontFaceDef;
	};
} // !TGen

#endif // !_TGEN_RENDERER_MODELRENDERPROPERTIES_H

