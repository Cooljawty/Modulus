#version 330

//Texture unit
uniform sampler2D Texture;

//Texture dimensions
uniform uvec2 TextureDimensions;

//Texture coordinate
in vec2 TexCoord;

//Final fragment's color
out vec4 FragColor;

void main(){
	FragColor = texture(Texture, vec2(TexCoord.x / TextureDimensions.x, TexCoord.y / TextureDimensions.y));
}
