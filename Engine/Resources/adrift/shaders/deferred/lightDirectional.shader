#section vertex

void main() {
	gl_Position = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}


#section fragment
uniform sampler2D normalMap, miscMap;


void directionalLight(int lightId, in vec3 normal, in float raiseTo, inout vec4 diffuse, inout vec4 specular) {
	float NdotVP, NdotHV, power;
	
	NdotVP = max(0.0, dot(normal, vec3(gl_LightSource[lightId].position)));
	NdotHV = max(0.0, dot(normal, vec3(gl_LightSource[lightId].halfVector)));
	
	if (NdotVP == 0.0)
		power = 0.0;
	else
		power = pow(NdotHV, raiseTo);

	diffuse += gl_LightSource[lightId].diffuse * NdotVP;
	specular += gl_LightSource[lightId].specular * power;
}


void main() {
	vec3 normal = normalize(vec3(texture2D(normalMap, gl_TexCoord[0].st)) * 2.0 - 1.0);	
	vec4 miscInfo = texture2D(miscMap, gl_TexCoord[0].st);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
	
	for (int i = 0; i < 8; ++i)
		directionalLight(i, normal, 40.0 * gl_LightSource[i].specular.a, diffuse, specular);
	
	gl_FragColor = (diffuse + specular * miscInfo * 10.0) * gl_Color;	// TODO: miscInfo.x ska bara användas
}

