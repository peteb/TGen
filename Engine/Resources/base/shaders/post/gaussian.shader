#section vertex

void main() {
	gl_Position = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}



#section fragment

uniform sampler2D colorMap;
uniform float texelSize;

void main() {
	vec4 sum = vec4(0.0);

	#ifdef HORIZONTAL
	sum += texture2D(colorMap, gl_TexCoord[0].st - vec2(texelSize * 2.0, 0.0)) * 0.0588235;
	sum += texture2D(colorMap, gl_TexCoord[0].st - vec2(texelSize, 0.0)) * 0.235294;
	sum += texture2D(colorMap, gl_TexCoord[0].st) * 0.583333;
	sum += texture2D(colorMap, gl_TexCoord[0].st + vec2(texelSize, 0.0)) * 0.235294;
	sum += texture2D(colorMap, gl_TexCoord[0].st + vec2(texelSize * 2.0, 0.0)) * 0.0588235;
	#elif defined(VERTICAL)
	sum += texture2D(colorMap, gl_TexCoord[0].st - vec2(0.0, texelSize * 2.0)) * 0.0588235;
	sum += texture2D(colorMap, gl_TexCoord[0].st - vec2(0.0, texelSize)) * 0.235294;
	sum += texture2D(colorMap, gl_TexCoord[0].st) * 0.5833333;
	sum += texture2D(colorMap, gl_TexCoord[0].st + vec2(0.0, texelSize)) * 0.235294;
	sum += texture2D(colorMap, gl_TexCoord[0].st + vec2(0.0, texelSize * 2.0)) * 0.0588235;
	#else
	#error "No direction for gaussian blur specified!"
	#endif
	
	gl_FragColor = sum;
	
}


