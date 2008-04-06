#section global
varying vec3 volumeCoords;
varying vec2 texCoord;
//varying vec3 lightInScreen;

#section vertex

void main() {
	gl_Position = ftransform();
	volumeCoords = (gl_ModelViewMatrix * gl_Vertex).xyz;
}



#section fragment

uniform sampler2D depthMap;

float ZPosFromDepthBufferValue(float depthBufferValue)
{
	float c0 = (1.0 - gl_DepthRange.far / gl_DepthRange.near) / 2.0;
	float c1 = (1.0 + gl_DepthRange.far / gl_DepthRange.near) / 2.0;
	
	return 1.0 / (c0 * depthBufferValue + c1);
	
	/*float a = gl_DepthRange.far / (gl_DepthRange.far - gl_DepthRange.near);
	float b = gl_DepthRange.far * gl_DepthRange.near / (gl_DepthRange.near - gl_DepthRange.far);
	
	
	return -b / (depthBufferValue - a);*/
	
	//return (1.0 / gl_DepthRange.near - 1.0 / depthBufferValue) / (1.0 / gl_DepthRange.near - 1.0 / gl_DepthRange.far);
}

//depth = (1.f / nearZ - 1.f / posz) / (1.f / nearZ - 1.f / farZ);

void main() {	
	float texDepth = texture2D(depthMap, gl_FragCoord.xy / 512.0).r * 2.0 - 1.0;
	
	vec4 pos = gl_ModelViewProjectionMatrixInverse * vec4(gl_FragCoord.xy / 256.0 - 1.0, texDepth, 1.0);
	pos /= pos.w;

	vec4 lightpos = gl_ProjectionMatrix * gl_LightSource[0].position;
	lightpos /= lightpos.w;
	
	// fšrkorta detta. plzzzzz..
	
	lightpos = gl_ModelViewProjectionMatrixInverse * lightpos;
	lightpos /= lightpos.w;
	
	
	// pos is in world space, lightpos is in world space
	// maxrange = 1.5 = 0 ljusstyrka
	//            0.0 = 1 ljusstyrka
	// clamp(1.0 - max(distance / maxrange, 0.0), 0.0, 1.0) = 
	/*
		
	*/
	
	float d = distance(pos.xyz, lightpos.xyz);
	float magic = clamp(1.0 - max(d / 2.5, 0.0), 0.0, 1.0);
	
	//if (magic < 0.5)
	//	magic = 0.0;
	
	gl_FragColor = vec4(gl_LightSource[0].diffuse.xyz * magic, 1.0);
}

