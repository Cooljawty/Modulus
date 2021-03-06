#version 330 core
uniform vec3 lightColor = vec3(1.0,1.0,1.0);

out vec4 FragColor;

void main(){
		FragColor = vec4(lightColor, 1.0);
}
 
