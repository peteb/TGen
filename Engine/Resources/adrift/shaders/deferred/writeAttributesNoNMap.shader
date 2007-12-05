
#section global
varying vec3 normal;

#section vertex
void main() {
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	normal = gl_NormalMatrix * gl_Normal;
}

#section fragment
uniform sampler2D colorMap, specularMap;

void main() {
	gl_FragData[0] = texture2D(colorMap, gl_TexCoord[0].st);
	gl_FragData[1] = vec4((normalize(normal) * 0.5 + 0.5).xyz, 1.0);
	gl_FragData[2] = vec4(texture2D(specularMap, gl_TexCoord[0].st * 3.0).r, 0.0, 0.0, 1.0); //vec4(texture2D(specularMap, gl_TexCoord[0].st).r, 0.0, 0.0, 0.0);	// we're only using the r component of specmap
}

