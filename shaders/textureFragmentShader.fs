#version 330 
struct Material{
	sampler2D texture;
};

//Texture unit
uniform Material material;

//Texture coordinate
in vec2 TexCoord;

//Final fragment's color
out vec4 FragColor;

void main(){
	FragColor = texture(material.texture, TexCoord);
}
