/*
 *  lodinfo.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 11/13/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_RENDERER_LODINFO_H
#define _TGEN_RENDERER_LODINFO_H

namespace TGen {
	struct LodInfo {
		LodInfo()
			: clipNear(0.1f)
			, clipFar(100.0f)
			, lodNear(0.1f)
			, lodFar(100.0f) {}
		
		
		scalar lodNear, lodFar, clipNear, clipFar;
	};
} // !TGen

#endif // !_TGEN_RENDERER_LODINFO_H

