#section vertex

void main() {
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}


#section fragment

void main() {
	gl_FragColor = gl_Color;
}

