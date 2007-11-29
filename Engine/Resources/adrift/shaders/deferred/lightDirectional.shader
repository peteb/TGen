#section vertex

void main() {
	gl_Position = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}


#section fragment
uniform sampler2D normalMap, miscMap;


//void directionalLight(in int lightId, in vec3 normal, in float raiseTo, inout vec4 diffuse, inout vec4 specular) {
//}


void main() {
	vec3 normal = normalize(vec3(texture2D(normalMap, gl_TexCoord[0].st)) * 2.0 - 1.0);	
	vec4 miscInfo = texture2D(miscMap, gl_TexCoord[0].st);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
	float NdotVP, NdotHV, power;
	
	#loop LIGHT_ID 0 #NUM_LIGHTS#
		NdotVP = max(0.0, dot(normal, vec3(gl_LightSource[#LIGHT_ID#].position)));
		NdotHV = max(0.0, dot(normal, vec3(gl_LightSource[#LIGHT_ID#].halfVector)));
	
		if (NdotVP == 0.0)
			power = 0.0;
		else
			power = pow(NdotHV, 400.0 * (1.0 - miscInfo.x));	// TODO: get frm material or map
	
		diffuse += gl_LightSource[#LIGHT_ID#].diffuse * NdotVP;
		specular += gl_LightSource[#LIGHT_ID#].specular * power;
	#end

	gl_FragColor = (diffuse + specular * miscInfo * 10.0) * gl_Color;	// TODO: miscInfo.x ska bara användas
}

