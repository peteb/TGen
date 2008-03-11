#section vertex 

void main() {
	gl_Position = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

#section fragment
uniform sampler2D colorMap, miscMap;
uniform float lumMin, lumMultiplier;
uniform bool killTrace;

void main() {
	vec4 color = texture2D(colorMap, gl_TexCoord[0].st);
	vec4 specular = texture2D(miscMap, gl_TexCoord[0].st);
	float lum = dot(color, vec4(0.299, 0.587, 0.114, 2.0));

	vec4 fixedColor = max(vec4(0.0), color - (lumMin - 0.2)) * lum * lumMultiplier;
	
	if (killTrace)
		gl_FragColor = vec4(fixedColor.rgb, 1.0);
	else
		gl_FragColor = fixedColor;
}

