/*
 *  writeAttributes.shader
 *  TGen Engine
 *
 *  Copyright 2007-2008 Peter Backman. All rights reserved.
 *
 *  Defines: 
 *    COLOR_MAP: use the color map, sampler colorMap
 *    NORMAL_MAP: use normal map, sampler normalMap
 *    SPECULAR_MAP: use specular map, sampler specularMap
 *    TRANSFORM_TEX: transform texture coordinates
 *    VERT_INTERPOL: interpolate between frames (needs nextVertex, nextNormal and frameTime)
 *    NULL_NORMALS: normals will be (0, 0, 0). useful when no lighting is wanted
 *    NO_AMBIENT: ambient lighting will not be used
 *    NO_GLOW: turn off glow
 *  
 *  Output:
 *    unit 0: color
 *    unit 1: normal
 *    unit 2: miscmap (r: specular)
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

#ifdef VERT_INTERPOL
	attribute vec3 nextVertex, nextNormal;
	uniform float frameTime;
#endif

void main() {
	#ifdef VERT_INTERPOL
		gl_Position = gl_ModelViewProjectionMatrix * vec4(mix(gl_Vertex.xyz, nextVertex, frameTime), 1.0);
		gl_FrontColor = gl_Color; // * frameTime;
	#else
		gl_FrontColor = gl_Color;
		gl_Position = ftransform();
	#endif
	
	
	    
	#if defined(COLOR_MAP) || defined(NORMAL_MAP) || defined(SPECULAR_MAP)
		#ifdef TRANSFORM_TEX
			gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;		
		#else
			gl_TexCoord[0] = gl_MultiTexCoord0;
		#endif
	#endif
	
	#ifdef NORMAL_MAP
		vec3 normal, bitangent;

		#ifdef VERT_INTERPOL
			normal = gl_NormalMatrix * (gl_Normal + (nextNormal - gl_Normal) * frameTime); 
		#else
			normal = gl_NormalMatrix * gl_Normal;
		#endif
		
		bitangent = cross(normal, tangent.xyz) * tangent.w;		
		tbnMatrix = mat3(tangent.xyz, bitangent, normal);
	#else
		#ifdef VERT_INTERPOL
			normal = gl_NormalMatrix * (gl_Normal + (nextNormal - gl_Normal) * frameTime);
		#else
			normal = gl_NormalMatrix * gl_Normal;
		#endif
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

// the tbn matrix will probably be skewed/unorthonormal if the surface isn't flat, send the axes as varyings
// and create matrix in frag

void main() {
	#ifdef COLOR_MAP
		#ifndef NO_AMBIENT
			vec4 color = texture2D(colorMap, gl_TexCoord[0].st) * gl_Color * gl_LightModel.ambient;
		#else
			vec4 color = texture2D(colorMap, gl_TexCoord[0].st) * gl_Color;		
		#endif
	#endif
	
	gl_FragData[0] = color;
	
	#ifdef NULL_NORMALS
		gl_FragData[1] = vec4(0.0, 0.0, 0.0, 0.0) * 0.5 + 0.5;
	#else
		#ifndef NORMAL_MAP
			gl_FragData[1] = vec4((normalize(normal) * 0.5 + 0.5).xyz, 1.0);
		#else
			vec3 normalFromMap = normalize(texture2D(normalMap, gl_TexCoord[0].st).xyz * 2.0 - 1.0);	// TODO: normalize ska kunna stängas av
			gl_FragData[1] = vec4((normalize(tbnMatrix * normalFromMap) * 0.5 + 0.5).xyz, 1.0);	
		#endif
	#endif
	
	vec4 miscValue = vec4(0.0, 0.0, 0.0, 1.0);
	
	#ifndef SPECULAR_MAP
		miscValue.r = 0.4;	
	#else
		miscValue.r = texture2D(specularMap, gl_TexCoord[0].st).r;
	#endif
	
	#ifndef NO_GLOW
		float lumMin = 0.2;
		float lumMultiplier = 5.0;
		float colorGlow = max(dot(color.rgb, vec3(0.299, 0.587, 0.114)), 0.0);
	
		float glowBase = (colorGlow - lumMin) * lumMultiplier;
	
		#ifdef MEGA_GLOW
			glowBase = colorGlow * 3.0;
		#endif

		miscValue.g = glowBase;	
		
	#endif
	
	gl_FragData[2] = miscValue;
}

