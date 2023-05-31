#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D Text;
uniform vec3 TextColor;

void main(){
	FragColor = vec4(TextColor, texture(Text, TexCoords).r);
}
