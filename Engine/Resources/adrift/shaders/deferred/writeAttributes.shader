
#section global
#ifndef NORMAL_MAP
varying vec3 normal;
#endif

#section vertex
void main() {
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
	

    #if defined(COLOR_MAP) || defined(NORMAL_MAP) || defined(SPECULAR_MAP)
	gl_TexCoord[0] = gl_MultiTexCoord0;
	#endif
	
	#ifndef NORMAL_MAP
	normal = gl_NormalMatrix * gl_Normal;
	#endif
}

#section fragment
uniform sampler2D colorMap;

#ifdef SPECULAR_MAP
uniform sampler2D specularMap;
#endif

void main() {
	// Render Color
	#ifdef COLOR_MAP
	gl_FragData[0] = texture2D(colorMap, gl_TexCoord[0].st);
	#endif
	
	#ifndef NORMAL_MAP
	gl_FragData[1] = vec4((normalize(normal) * 0.5 + 0.5).xyz, 1.0);
	#else
	#error "Doesn't support normal maps yet!"
	#endif
	
	// Render misc
	#ifndef SPECULAR_MAP
	gl_FragData[2] = vec4(1.0, 0.0, 0.0, 1.0);	
	#else
	gl_FragData[2] = vec4(texture2D(specularMap, gl_TexCoord[0].st).r, 0.0, 0.0, 1.0); //vec4(texture2D(specularMap, gl_TexCoord[0].st).r, 0.0, 0.0, 0.0);	// we're only using the r component of specmap
	#endif
}

