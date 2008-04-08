#section global
varying vec3 volumeCoords;
varying vec2 texCoord;
varying vec4 lightpos;
varying vec3 eyeToLight;

#section vertex

uniform vec3 eye;		// eye position in world

void main() {
	gl_Position = ftransform();
	volumeCoords = (gl_ModelViewMatrix * gl_Vertex).xyz;

	lightpos = gl_ProjectionMatrix * gl_LightSource[0].position;
	lightpos /= lightpos.w;
	
	lightpos = gl_ModelViewProjectionMatrixInverse * lightpos;
	lightpos /= lightpos.w;
	
	eyeToLight = eye - lightpos.xyz;
}



#section fragment

uniform sampler2D depthMap;
uniform sampler2D normalMap;
uniform vec2 rtSize;	// render target size
uniform vec3 eye;		// eye position in world

void main() {	
	float texDepth = texture2D(depthMap, gl_FragCoord.xy / rtSize).r * 2.0 - 1.0;
	vec3 normal = normalize(texture2D(normalMap, gl_FragCoord.xy / rtSize).xyz * 2.0 - 1.0);	
	
	vec4 pos = gl_ModelViewProjectionMatrixInverse * vec4(gl_FragCoord.xy / (rtSize / 2.0) - 1.0, texDepth, 1.0);
	pos /= pos.w;

	
	vec3 lightToFrag = normalize(lightpos.xyz - pos.xyz);
	vec3 view = normalize(eye - pos.xyz);
	vec3 H = normalize((lightToFrag + view) / 2.0);
	
	float NdotL = max(0.0, dot(normal, lightToFrag));
	
	float d = 1.0 - max(distance(pos.xyz, lightpos.xyz) / 2.5, 0.0);   // 0..1 in distance range
	float diffuse = d * NdotL;
	float specular = max(0.0, pow(dot(normal, H), 70.0)) * d;
	
	//gl_FragColor = vec4(NdotL, NdotL, NdotL, 1.0);
	gl_FragColor = vec4(gl_LightSource[0].diffuse.rgb * diffuse + gl_LightSource[0].specular.rgb * specular, 1.0);
}

