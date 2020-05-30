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
	vec3 direction;		// Direção da luz, espaço do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
};

uniform DirectionalLight directionalLight; // Fonte de luz direcional

// Estrutura de uma fonte de luz pontual
struct PointLight	{
	vec3 position;		// Posição do ponto de luz, espaço do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenuação constante
	float linear;		// Coeficiente de atenuação linear
	float quadratic;	// Coeficiente de atenuação quadrática
};

uniform PointLight pointLight; // Duas fontes de luz pontual

// Estrutura de uma fonte de luz cónica
struct SpotLight {
	vec3 position;		// Posição do foco de luz, espaço do mundo
	
	vec3 ambient;		// Componente de luz ambiente
	vec3 diffuse;		// Componente de luz difusa
	vec3 specular;		// Componente de luz especular
	
	float constant;		// Coeficiente de atenuação constante
	float linear;		// Coeficiente de atenuação linear
	float quadratic;	// Coeficiente de atenuação quadrática

	float spotCutoff;
	vec3 spotDirection;
};

uniform SpotLight spotLight; // Fonte de luz cónica

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

	// Cálculo da componente emissiva do material.
	vec4 emissive = vec4(material.emissive, 1.0);

	// Cálculo do efeito da iluminação no fragmento.
	vec4 light[4];
	// Contribuição da fonte de luz ambiente
	if (bAmbient == 1)
	light[0] = calcAmbientLight(ambientLight);
	else
	light[0] = vec4(0.0);

	// Contribuição da fonte de luz direcional
	if (bDirectional == 1)
	light[1] = calcDirectionalLight(directionalLight);
	else
	light[1] = vec4(0.0);

	// Contribuição de cada fonte de luz Pontual
	if (bPoint == 1)
	light[2] = calcPointLight(pointLight);
	else
	light[2] = vec4(0.0);

	// Contribuição da fonte de luz cónica
	if (bSpot == 1)
	light[3] = calcSpotLight(spotLight);
	else
	light[3] = vec4(0.0);

	// Cálculo da cor final do fragmento.
	// Com CubeMap
	fColor = (emissive + light[0] + light[1] + light[2] + light[3]) * texture(u_Texture, v_TexCoord);
	// Com cor de fragmento
	//fColor = (emissive + light[0] + light[1] + light[2] + light[3]) * vec4(1.0, 1.0, 1.0, 1.0);
}


vec4 calcAmbientLight(AmbientLight light) {
	// Cálculo da contribuição da fonte de luz ambiente global, para a cor do objeto.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light) {
	// Cálculo da reflexão da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	// Cálculo da reflexão da componente da luz difusa.
	vec3 lightDirectionEyeSpace = (u_View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace); // Direção inversa à da direção luz.
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
	
	// Cálculo da reflexão da componente da luz especular.
	// Como os cálculos estão a ser realizados nas coordenadas do olho, então a câmara está na posição (0,0,0).
	// Resulta então um vetor V entre os pontos (0,0,0) e vPositionEyeSpace:
	//		V = (0,0,0) - vPositionEyeSpace = (0-vPositionEyeSpace.x, 0-vPositionEyeSpace.y, 0-vPositionEyeSpace.z)
	// Que pode ser simplificado como:
	//		- vPositionEyeSpace
	vec3 V = normalize(-vPositionEyeSpace);
	//vec4 H = normalize(L + V);	// Modelo Blinn-Phong
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	// Modelo Blinn-Phong
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

	// Cálculo da contribuição da fonte de luz direcional para a cor final do fragmento.
	return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light) {
	// Cálculo da reflexão da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	// Cálculo da reflexão da componente da luz difusa.
	//vec3 lightPositionEyeSpace = mat3(View) * light.position;
	vec3 lightPositionEyeSpace = (u_View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

	// Cálculo da reflexão da componente da luz especular.
	// Como os cálculos estão a ser realizados nas coordenadas do olho, então a câmara está na posição (0,0,0).
	// Resulta então um vetor V entre os pontos (0,0,0) e vPositionEyeSpace:
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
	float dist = length(mat3(u_View) * light.position - vPositionEyeSpace);	// Cálculo da distância entre o ponto de luz e o vértice
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// Cálculo da contribuição da fonte de luz pontual para a cor final do fragmento.
	return (attenuation * (ambient + diffuse + specular));
}

vec4 calcSpotLight(SpotLight light) {
	// Cálculo da reflexão da componente da luz ambiente.
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);


	//Vê se a luz está dentro do cone
	vec3 lightDir = normalize(light.position - vPositionEyeSpace);
	float theta = dot(lightDir, normalize(-light.spotDirection));

	if(theta > light.spotCutoff){
		// Cálculo da reflexão da componente da luz difusa.
		//vec3 lightPositionEyeSpace = mat3(View) * light.position;
		vec3 lightPositionEyeSpace = (u_View * vec4(light.position, 1.0)).xyz;
		vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
		vec3 N = normalize(vNormalEyeSpace);
		float NdotL = max(dot(N, L), 0.0);
		diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

		// Cálculo da reflexão da componente da luz especular.
		// Como os cálculos estão a ser realizados nas coordenadas do olho, então a câmara está na posição (0,0,0).
		// Resulta então um vetor V entre os pontos (0,0,0) e vPositionEyeSpace:
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
		float dist = length(mat3(u_View) * light.position - vPositionEyeSpace);	// Cálculo da distância entre o ponto de luz e o vértice
		float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

		diffuse *= attenuation;
		specular *= attenuation;
	}
	return (ambient + diffuse + specular);
}
