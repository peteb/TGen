#section vertex 

void main() {
	gl_Position = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

#section fragment
uniform sampler2D colorMap, miscMap;

void main() {
	vec4 color = texture2D(colorMap, gl_TexCoord[0].st);
	vec4 specular = texture2D(miscMap, gl_TexCoord[0].st);
	float lum = dot(color, vec4(0.3, 0.59, 0.11, 1.0));
	
	gl_FragColor = vec4((max(vec4(0.0), color - 0.8) * lum * 1.5).rgb, 1.0);
}

