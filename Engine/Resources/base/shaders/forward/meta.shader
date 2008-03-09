
#section vertex

void main() {
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
}


#section fragment

void main() {
	gl_FragColor = gl_Color;
}
