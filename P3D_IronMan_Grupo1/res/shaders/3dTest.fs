#version 330 core

layout(location = 0) out vec4 color;

in vec3 vPositionEyeSpace;
in vec3 vNormalEyeSpace;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

// Estrutura da fonte de luz ambiente global
struct AmbientLight {
	vec3 ambient;	// Componente de luz ambiente global
};

uniform AmbientLight ambientLight; // Fonte de luz ambiente global

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
}
