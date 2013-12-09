#extension GL_EXT_gpu_shader4 : require

varying vec3 normal, lightDir, eyeVec;
varying vec2 vTexCoord;

uniform sampler2D earthColor;
uniform sampler2D earthBump;
uniform sampler2D earthSpecular;
uniform sampler2D earthNormal;
uniform sampler2D earthNightLights;

float diffuseGain = 1.0;
float diffusePow = 1.0;

float specularGain = 0.4;
float specularClamp = 0.2;
float specularPow = 5.0;

void main( void ){

	// TEX /////////////////////////////////////
	vec4 colorT = vec4( 0.6 * texture2D(earthColor, vTexCoord).rgb, 1.0); //less rgb gain to avoid burn in with blur overlay
	vec4 bumpT = texture2D(earthBump, vTexCoord) ;
	vec4 specularT = texture2D(earthSpecular, vTexCoord);
	vec4 normalT = texture2D(earthNormal, vTexCoord);
	vec4 nightLightsT = texture2D(earthNightLights, vTexCoord);

	vec3 L = normalize(lightDir);
	vec3 Nclean = normalize(normal);
	vec3 N = normalize(Nclean + (2.0 * normalT.xyz) - 1.0); 
	
	vec4 ambientV = vec4(0.0);
	vec4 diffuseV = vec4(0.0);
	vec4 specularV = vec4(0.0);
	
	// AMBIENT /////////////////////////////////
	ambientV = (gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
	
	// DIFFUSE /////////////////////////////////
	
	float night = clamp(5.0 * dot(Nclean, L), 0.0, 1.0);
	
	float lambert = dot(N, L);
	
	if(lambert  > 0.0){

		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);

		diffuseV = diffuseGain * gl_LightSource[0].diffuse * colorT * pow(lambert , diffusePow );

		// SPECULAR /////////////////////////////////
		float specular = specularGain * clamp( pow( max(dot(R,E), 0.0), specularPow ), 0.0, specularClamp);
		specularV = specular * specularT;	
	
	}

	vec4 dayV = ambientV + diffuseV + specularV;
	//vec4 dayV = vec4(0.0, 0.0, 0.0, 1.0) + specularV ;
	vec4 nightV = vec4(0.0, 0.0, 0.0, 1.0) + nightLightsT * 0.5;
	
	gl_FragColor = mix(dayV, nightV, 1.0 - night);
	
	//gl_FragColor = vec4(N, 1.0);
	//gl_FragColor = specularV;
	
}

