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

namespace TGen {
	class LightMaterial {
	public:
		LightMaterial() : shininess(20.0f), diffuse(TGen::Color::White), specular(TGen::Color(0.5f, 0.5f, 0.5f)) {}
		
		TGen::Color diffuse, specular;
		scalar shininess;
	};
	
} // !TGen

#endif // !_TGEN_LIGHTMATERIAL_H
