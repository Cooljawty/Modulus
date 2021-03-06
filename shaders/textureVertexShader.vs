#version 330
//Transformation Matracies
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

//Position attribute
layout(location = 0) in vec2 position;

//Texture coordinate attribute
layout(location = 1) in vec2 texture;
out vec2 TexCoord;

void main(){
	TexCoord = texture;
	
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix *  vec4(position, 0.f, 1.f);
}

