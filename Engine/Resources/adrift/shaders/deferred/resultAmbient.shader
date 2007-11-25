#section vertex

void main() {
	gl_Position = gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}


#section fragment
uniform sampler2D colorMap;

void main() {
	gl_FragColor = texture2D(colorMap, gl_TexCoord[0].st) * gl_LightModel.ambient;

}
