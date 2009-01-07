/*
 *  lightmaterial.h
 *  TGen Graphics
 *
 *  Created by Peter Backman on 11/9/08.
 *  Copyright 2008 Peter Backman. All rights reserved.
 *
 */

#ifndef _TGEN_LIGHTMATERIAL_H
#define _TGEN_LIGHTMATERIAL_H

#include "color.h"

namespace TGen {
	class LightMaterial {
	public:
		LightMaterial();
		
		TGen::Color diffuse, specular;
		scalar shininess;
	};
	
} // !TGen

#endif // !_TGEN_LIGHTMATERIAL_H
