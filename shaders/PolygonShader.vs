#version 330 core
uniform mat4 PVMatrix;
uniform mat4 ModelMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out V_DATA{
	vec3 FragPosition;
	vec3 VertexNormal;
	vec2 TextureCoords;
} vs_out;

void main(){
	gl_Position =  PVMatrix * ModelMatrix * vec4(position, 1.0);
	vs_out.FragPosition = vec3(ModelMatrix * vec4(position, 1.0));
	vs_out.VertexNormal = normal;
	vs_out.TextureCoords = texcoord;
}
