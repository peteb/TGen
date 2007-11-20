#section vertex

void main() {
	gl_Position = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}



#section fragment

uniform sampler2D colorMap;
const float texelWidth = 1.0 / 256.0;

void main() {
	vec4 sum = vec4(0.0);

	//sum += texture2D(colorMap, gl_TexCoord[0].st - vec2(texelWidth, 0.0)) * 0.33333;
//	sum += texture2D(colorMap, gl_TexCoord[0].st) * 0.666667;
//	sum += texture2D(colorMap, gl_TexCoord[0].st + vec2(texelWidth, 0.0)) * 0.33333;


	sum += texture2D(colorMap, gl_TexCoord[0].st - vec2(0.0, texelWidth * 2.0)) * 0.0588235;
	sum += texture2D(colorMap, gl_TexCoord[0].st - vec2(0.0, texelWidth)) * 0.235294;
	sum += texture2D(colorMap, gl_TexCoord[0].st) * 0.5833333;
	sum += texture2D(colorMap, gl_TexCoord[0].st + vec2(0.0, texelWidth)) * 0.235294;
	sum += texture2D(colorMap, gl_TexCoord[0].st + vec2(0.0, texelWidth * 2.0)) * 0.0588235;

	
	gl_FragColor = sum;
	
}


