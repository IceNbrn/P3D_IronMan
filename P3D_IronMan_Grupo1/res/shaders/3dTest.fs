#version 330 core


in vec3 vPositionEyeSpace;
in vec3 vNormalEyeSpace;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// Estrutura da fonte de luz ambiente global
struct AmbientLight {
	vec3 ambient;	// Componente de luz ambiente global
};

uniform AmbientLight ambientLight; // Fonte de luz ambiente global

// Estrutura de uma fonte de luz direcional
struct DirectionalLight	{
	vec3 direction;		// Dire?o da luz, espa? do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
};

uniform DirectionalLight directionalLight; // Fonte de luz direcional

// Estrutura de uma fonte de luz pontual
struct PointLight	{
	vec3 position;		// Posi?o do ponto de luz, espa? do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenua?o constante
	float linear;		// Coeficiente de atenua?o linear
	float quadratic;	// Coeficiente de atenua?o quadr?ica
};

uniform PointLight pointLight; // Duas fontes de luz pontual

// Estrutura de uma fonte de luz c?ica
struct SpotLight {
	vec3 position;		// Posi?o do foco de luz, espa? do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenua?o constante
	float linear;		// Coeficiente de atenua?o linear
	float quadratic;	// Coeficiente de atenua?o quadr?ica

	float spotCutoff;
	vec3 spotDirection;
};

uniform SpotLight spotLight; // Fonte de luz c?ica

struct Material{
	vec3 emissive;
	vec3 ambient;		// Ka
	vec3 diffuse;		// kd
	vec3 specular;		// ke
	float shininess;
};

uniform Material material;

layout (location = 0) out vec4 fColor; // Cor final do fragmento

vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light);
vec4 calcPointLight(PointLight light);
vec4 calcSpotLight(SpotLight light);

uniform	int	bAmbient;
uniform int bDirectional;
uniform int bPoint;
uniform int bSpot;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);

	// C?culo da componente emissiva do material.
	vec4 emissive = vec4(material.emissive, 1.0);

	// C?culo do efeito da ilumina?o no fragmento.
	vec4 light[4];
	// Contribui?o da fonte de luz ambiente
	if (bAmbient == 1)
		light[0] = calcAmbientLight(ambientLight);
	else
		light[0] = vec4(0.0);

	// Contribui?o da fonte de luz direcional
	if (bDirectional == 1)
		light[1] = calcDirectionalLight(directionalLight);
	else
		light[1] = vec4(0.0);

	// Contribui?o de cada fonte de luz Pontual
	if (bPoint == 1)
		light[2] = calcPointLight(pointLight);
	else
		light[2] = vec4(0.0);

	// Contribui?o da fonte de luz c?ica
	if (bSpot == 1)
		light[3] = calcSpotLight(spotLight);
	else
		light[3] = vec4(0.0);

	// C?culo da cor final do fragmento.
	// Com CubeMap
	fColor = (emissive + light[0] + light[1] + light[2] + light[3]) * texture(u_Texture, v_TexCoord);
	// Com cor de fragmento
	//fColor = (emissive + light[0] + light[1] + light[2] + light[3]) * vec4(1.0, 1.0, 1.0, 1.0);
}


vec4 calcAmbientLight(AmbientLight light) {
	// C?culo da contribui?o da fonte de luz ambiente global, para a cor do objeto.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light) {
	// C?culo da reflex? da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	// C?culo da reflex? da componente da luz difusa.
	vec3 lightDirectionEyeSpace = (u_View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace); // Dire?o inversa ?da dire?o luz.
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
	
	// C?culo da reflex? da componente da luz especular.
	// Como os c?culos est? a ser realizados nas coordenadas do olho, ent? a c?ara est?na posi?o (0,0,0).
	// Resulta ent? um vetor V entre os pontos (0,0,0) e vPositionEyeSpace:
	//		V = (0,0,0) - vPositionEyeSpace = (0-vPositionEyeSpace.x, 0-vPositionEyeSpace.y, 0-vPositionEyeSpace.z)
	// Que pode ser simplificado como:
	//		- vPositionEyeSpace
	vec3 V = normalize(-vPositionEyeSpace);
	//vec4 H = normalize(L + V);	// Modelo Blinn-Phong
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	// Modelo Blinn-Phong
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

	// C?culo da contribui?o da fonte de luz direcional para a cor final do fragmento.
	return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light) {
	// C?culo da reflex? da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	// C?culo da reflex? da componente da luz difusa.
	//vec3 lightPositionEyeSpace = mat3(View) * light.position;
	vec3 lightPositionEyeSpace = (u_View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

	// C?culo da reflex? da componente da luz especular.
	// Como os c?culos est? a ser realizados nas coordenadas do olho, ent? a c?ara est?na posi?o (0,0,0).
	// Resulta ent? um vetor V entre os pontos (0,0,0) e vPositionEyeSpace:
	//		V = (0,0,0) - vPositionEyeSpace = (0-vPositionEyeSpace.x, 0-vPositionEyeSpace.y, 0-vPositionEyeSpace.z)
	// Que pode ser simplificado como:
	//		- vPositionEyeSpace
	vec3 V = normalize(-vPositionEyeSpace);
	//vec4 H = normalize(L + V);	// Modelo Blinn-Phong
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	// Modelo Blinn-Phong
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
	
	// attenuation
	float dist = length(mat3(u_View) * light.position - vPositionEyeSpace);	// C?culo da dist?cia entre o ponto de luz e o v?tice
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// C?culo da contribui?o da fonte de luz pontual para a cor final do fragmento.
	return (attenuation * (ambient + diffuse + specular));
}

vec4 calcSpotLight(SpotLight light) {
	// C?culo da reflex? da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);


	//V?se a luz est?dentro do cone
	vec3 lightDir = normalize(light.position - vPositionEyeSpace);
	float theta = dot(lightDir, normalize(-light.spotDirection));

	if(theta > light.spotCutoff){
		// C?culo da reflex? da componente da luz difusa.
		//vec3 lightPositionEyeSpace = mat3(View) * light.position;
		vec3 lightPositionEyeSpace = (u_View * vec4(light.position, 1.0)).xyz;
		vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
		vec3 N = normalize(vNormalEyeSpace);
		float NdotL = max(dot(N, L), 0.0);
		diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

		// C?culo da reflex? da componente da luz especular.
		// Como os c?culos est? a ser realizados nas coordenadas do olho, ent? a c?ara est?na posi?o (0,0,0).
		// Resulta ent? um vetor V entre os pontos (0,0,0) e vPositionEyeSpace:
		//		V = (0,0,0) - vPositionEyeSpace = (0-vPositionEyeSpace.x, 0-vPositionEyeSpace.y, 0-vPositionEyeSpace.z)
		// Que pode ser simplificado como:
		//		- vPositionEyeSpace
		vec3 V = normalize(-vPositionEyeSpace);
		//vec4 H = normalize(L + V);	// Modelo Blinn-Phong
		vec3 R = reflect(-L, N);
		float RdotV = max(dot(R, V), 0.0);
		//float NdotH = max(dot(N, H), 0.0);	// Modelo Blinn-Phong
		specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
		
		// attenuation
		float dist = length(mat3(u_View) * light.position - vPositionEyeSpace);	// C?culo da dist?cia entre o ponto de luz e o v?tice
		float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

		diffuse *= attenuation;
		specular *= attenuation;
	}
	return (ambient + diffuse + specular);
}
