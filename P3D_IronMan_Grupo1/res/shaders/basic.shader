#shader vertex
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 v_Normal;

out vec3 vPositionEyeSpace;
out vec3 vNormalEyeSpace;
out vec2 v_TexCoord;

uniform mat4 u_ModelView;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat3 u_NormalMatrix;

void main()
{
	vPositionEyeSpace = (u_ModelView * vec4(position, 1.0)).xyz;

	vNormalEyeSpace = normalize(u_NormalMatrix * v_Normal);

	v_TexCoord = texCoord;
	gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0f);
}

#shader fragment
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

uniform AmbientLight u_AmbientLight;
void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
}
