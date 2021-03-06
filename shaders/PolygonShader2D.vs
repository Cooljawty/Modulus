#version 330 core
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;

out vec4 VertexColor;

void main(){
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 0.f, 1.f);
	VertexColor = color;
}
