#version 330 
//Texture unit
uniform sampler2D Sample;

//Texture coordinate
in vec2 TexCoord;

//Final fragment's color
out vec4 FragColor;

void main(){
	FragColor = texture(Sample, TexCoord);
}
