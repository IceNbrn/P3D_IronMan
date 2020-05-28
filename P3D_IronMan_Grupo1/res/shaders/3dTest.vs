#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
in vec3 vNormal;// Normal do vértice

out vec2 v_TexCoord;
out vec3 vPositionEyeSpace;
out vec3 vNormalEyeSpace;

uniform mat4 u_Model;
uniform mat4 ModelView;		// View * Model
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat3 NormalMatrix;

void main()
{
	v_TexCoord = texCoord;
	gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0f);

	// Posição do vértice em coordenadas do olho.
	vPositionEyeSpace = (ModelView * vec4(position, 1.0)).xyz;

	// Transformar a normal do vértice.
	vNormalEyeSpace = normalize(NormalMatrix * vNormal);

}