#section global
varying vec4 color;

#section vertex
void main() {
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	color = gl_Color;
}

#section fragment
uniform sampler2D texture, environment;

void main() {
	gl_FragColor = clamp(texture2D(texture, gl_TexCoord[0].st) * color, 0.0, 1.0);
}
