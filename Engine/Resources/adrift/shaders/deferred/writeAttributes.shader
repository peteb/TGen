/*
 *  writeAttributes.shader
 *  TGen Engine
 *
 *  Copyright 2007-2008 Peter Backman. All rights reserved.
 *
 *  Defines: COLOR_MAP, NORMAL_MAP, SPECULAR_MAP
 */
 
#section global

#ifdef NORMAL_MAP
	varying mat3 tbnMatrix;
#else
	varying vec3 normal;
#endif



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
#section vertex


#ifdef NORMAL_MAP
	attribute vec4 tangent;
#endif

void main() {
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
	    
	#if defined(COLOR_MAP) || defined(NORMAL_MAP) || defined(SPECULAR_MAP)
		gl_TexCoord[0] = gl_MultiTexCoord0;
	#endif
	
	#ifdef NORMAL_MAP
		vec3 normal = gl_NormalMatrix * gl_Normal;
		vec3 bitangent = cross(normal, tangent.xyz) * tangent.w;
		
		tbnMatrix = mat3(tangent.xyz, bitangent, normal);
	#else
		normal = gl_NormalMatrix * gl_Normal;
	#endif
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
#section fragment

#ifdef COLOR_MAP
	uniform sampler2D colorMap;
#endif

#ifdef SPECULAR_MAP
	uniform sampler2D specularMap;
#endif

#ifdef NORMAL_MAP
	uniform sampler2D normalMap;
#endif

// the tbn matrix will probably be skewed/unorthonormal if the surface isn't flat
void main() {
	#ifdef COLOR_MAP
		gl_FragData[0] = texture2D(colorMap, gl_TexCoord[0].st);
	#endif
	
	#ifndef NORMAL_MAP
		gl_FragData[1] = vec4((normalize(normal) * 0.5 + 0.5).xyz, 1.0);

	#else
		vec3 normalFromMap = normalize(texture2D(normalMap, gl_TexCoord[0].st).xyz * 2.0 - 1.0);	// TODO: normalize ska kunna stängas av
		gl_FragData[1] = vec4((normalize(tbnMatrix * normalFromMap) * 0.5 + 0.5).xyz, 1.0);	
			
	#endif
	
	#ifndef SPECULAR_MAP
		gl_FragData[2] = vec4(0.4, 0.0, 0.0, 1.0);	
	#else
		gl_FragData[2] = vec4(texture2D(specularMap, gl_TexCoord[0].st).r, 0.0, 0.0, 1.0); //vec4(texture2D(specularMap, gl_TexCoord[0].st).r, 0.0, 0.0, 0.0);	// we're only using the r component of specmap
	#endif
}

