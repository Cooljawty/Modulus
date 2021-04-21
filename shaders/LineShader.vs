#version 330 core
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 color;

out V_DATA{
	vec3 VertexColor;
}vs_out;

void main(){
	gl_Position =  ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
	vs_out.VertexColor = color;
}
