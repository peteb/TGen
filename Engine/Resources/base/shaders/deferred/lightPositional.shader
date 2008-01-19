#section global
varying vec3 volumeCoords;
varying vec2 texCoord;

#section vertex

/*void main() {
	vec4 transformed = ftransform();
	gl_Position = vec4(transformed.xy, 0.0, 1.0);
	vec3 fixedPos = transformed.xyz / gl_Position.w;

	gl_TexCoord[0] = vec4(fixedPos * 0.5 + 0.5, 0.0);
}*/

void main() {
	gl_Position = ftransform();
	volumeCoords = (gl_ModelViewMatrix * gl_Vertex).xyz;
	texCoord = gl_Position.xy;
}

#section fragment

#extension GL_ARB_texture_rectangle : enable
uniform sampler2D depthMap;

void main() {
	float depthParamA, depthParamB;
	depthParamA = gl_DepthRange.far / (gl_DepthRange.far - gl_DepthRange.near);
	depthParamB = gl_DepthRange.far * gl_DepthRange.near / (gl_DepthRange.near - gl_DepthRange.far);
	
	float fragDepth = -depthParamB / ( texture2D( depthMap, gl_FragCoord.xy / 512.0).r - depthParamA ); // Convert from non-linear to linear
	vec3 fragLocation = vec3( volumeCoords.xy * fragDepth / volumeCoords.z , fragDepth); // Calculate fragments location in view-space
	
	vec3 worldLocation = (gl_ModelViewMatrixInverse * vec4(fragLocation, 0.0)).xyz;
	
	float d = distance(worldLocation, vec3(0.0, 0.0, 0.0));
	
	gl_FragColor = vec4(texture2D( depthMap, gl_FragCoord.xy / 512.0).r, 0.0, 0.0, 1.0);
}

