/*
 *  materialsource.h
 *  TGen Renderer
 *
 *  Created by Peter Backman on 8/13/07.
 *  Copyright 2007 Peter Backman. All rights reserved.
 *
 */

//// summary ///////////////////////////////////////////////////////////////////////////////////////
// A class that implements the MaterialSource interface can be used to link Faces with materials.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TGEN_RENDERER_MATERIALSOURCE_H
#define _TGEN_RENDERER_MATERIALSOURCE_H

#include <tgen_core.h>

namespace TGen {
	class Material;
	
	class MaterialSource {
	public:
		MaterialSource() {}
		virtual ~MaterialSource() {}
		
		virtual TGen::Material * getMaterial(const std::string & name) abstract;
	};
} // !TGen

#endif // !_TGEN_RENDERER_MATERIALSOURCE_H
