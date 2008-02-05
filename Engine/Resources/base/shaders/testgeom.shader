
#set geom_input triangles
#set geom_output triangles



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
#section global



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
#section vertex

void main() {
	gl_Position = ftransform();
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
#section geometry

#version 120
#extension GL_EXT_geometry_shader4 : enable

void main() {
	int i;
	
	for (i = 0; i < gl_VerticesIn; ++i) {
		gl_Position = gl_PositionIn[i];
		EmitVertex();
	}
	
	EndPrimitive();
	
	for (i = 0; i < gl_VerticesIn; ++i) {
		gl_Position = gl_PositionIn[i];
		gl_Position.xy = gl_Position.yx;
	}

	EndPrimitive();
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
#section fragment

void main() {
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}


