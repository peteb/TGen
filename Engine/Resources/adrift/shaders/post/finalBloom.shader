#section vertex
void main() {
	gl_Position = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

#section fragment
uniform sampler2D colorMap, bloomMap;

void main() {
	gl_FragColor = texture2D(colorMap, gl_TexCoord[0].st) + texture2D(bloomMap, gl_TexCoord[0].st);
}

