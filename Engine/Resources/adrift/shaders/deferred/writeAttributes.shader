
#section global
#ifdef NO_NORMAL_MAP
varying vec3 normal;
#endif

#section vertex
void main() {
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	#ifdef NO_NORMAL_MAP
	normal = gl_NormalMatrix * gl_Normal;
	#endif
}

#section fragment
uniform sampler2D colorMap, specularMap;

void main() {
	// Render Color
	gl_FragData[0] = texture2D(colorMap, gl_TexCoord[0].st);
	
	#ifdef NO_NORMAL_MAP
	gl_FragData[1] = vec4((normalize(normal) * 0.5 + 0.5).xyz, 1.0);
	#endif
	
	// Render misc
	gl_FragData[2] = vec4(texture2D(specularMap, gl_TexCoord[0].st).r, 0.0, 0.0, 1.0); //vec4(texture2D(specularMap, gl_TexCoord[0].st).r, 0.0, 0.0, 0.0);	// we're only using the r component of specmap
}

