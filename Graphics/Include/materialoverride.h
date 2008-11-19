/*
 *  materialoverride.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 11/9/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_GRAPHICS_MATERIALOVERRIDE_H
#define _TGEN_GRAPHICS_MATERIALOVERRIDE_H

namespace TGen {
	class Renderer;
	
	class MaterialOverride {
	public:
		virtual ~MaterialOverride() {}
		
		virtual void overrideMaterial(TGen::Renderer & renderer, int param) const abstract;
	};
	
} // !TGen

#endif // !_TGEN_GRAPHICS_MATERIALOVERRIDE_H
