#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 v_Normal;

uniform mat4 u_ModelView;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat3 u_NormalMatrix;
uniform int u_bDeformActive;
uniform float u_Time;

out vec3 vPositionEyeSpace;
out vec3 vNormalEyeSpace;
out vec2 v_TexCoord;


void main()
{
	vPositionEyeSpace = (u_ModelView * vec4(position, 1.0)).xyz;

	vNormalEyeSpace = normalize(u_NormalMatrix * v_Normal);

	v_TexCoord = texCoord;
	if(u_bDeformActive == 1)
	{
		vec3 v_DeformPosition;
		v_DeformPosition = position + (v_Normal * (cos(u_Time * 0.001f) * 0.05f));
		gl_Position = u_Projection * u_View * u_Model * vec4(v_DeformPosition, 1.0f);
	}
	else
	{
		gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0f);
	}
}