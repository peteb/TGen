#section vertex

void main() {
	vec4 transformed = ftransform();
	gl_Position = vec4(transformed.xy, 0.0, 1.0);
	vec3 fixedPos = transformed.xyz / gl_Position.w;

	gl_TexCoord[0] = vec4(fixedPos * 0.5 + 0.5, 0.0);
}

#section fragment
uniform sampler2D normalMap, miscMap, depthMap, colorMap;

void main() {
   vec4 normal = /*normalize( */ texture2D(normalMap, gl_TexCoord[0].st); // * 2.0 - 1.0);
   vec4 miscInfo = texture2D(miscMap, gl_TexCoord[0].st);
	vec4 depth = texture2D(depthMap, gl_TexCoord[0].st);
	

	vec4 res = normal * 0.3 + miscInfo * 0.3 + depth * 0.3 + texture2D(colorMap, gl_TexCoord[0].st) * 0.3;
	gl_FragColor = texture2D(normalMap, gl_TexCoord[0].st);
}

