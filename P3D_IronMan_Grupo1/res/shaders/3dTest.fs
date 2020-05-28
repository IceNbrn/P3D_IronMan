#version 330 core

layout(location = 0) out vec4 color;

in vec3 vPositionEyeSpace;
in vec3 vNormalEyeSpace;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

struct AmbientLight {
	vec3 ambient;
};

struct Material{
	vec3 emissive;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform AmbientLight u_AmbientLight;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
}
