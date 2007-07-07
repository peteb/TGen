#section global

#section vertex
void main() {
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	gl_TexCoord[2] = gl_MultiTexCoord2;

}

#section fragment
uniform sampler2D tex, environment;

void main() {
	gl_FragColor = texture2D(environment, gl_TexCoord[0].st);
	
	vec4 hej = texture2D(tex, gl_TexCoord[0].st);
}
