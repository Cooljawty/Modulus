#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;
out vec2 TexCoords;

uniform mat4 ProjectionMatrix;

void main(){
	gl_Position = ProjectionMatrix * vec4(pos, 0.0, 1.0);
	TexCoords = uv;
}
