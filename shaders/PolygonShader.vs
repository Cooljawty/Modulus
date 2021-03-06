#version 330 core
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texcoord;

out VS_DATA{
	vec3 FragPosition;
	vec4 VertexColor;
	vec2 TextureCoords;
} vs_out;

void main(){
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.f);
	vs_out.FragPosition = vec3(ModelMatrix * vec4(position, 1.0));
	vs_out.VertexColor = color;
	vs_out.TextureCoords = texcoord;
}
