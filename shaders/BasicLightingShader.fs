#version 330 core
struct Material{
		sampler2D diffuse;
		sampler2D specular;
		float shininess;
};

struct Light{
	vec3 ambiant;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPosition;

in GS_DATA{
	vec3 VertexNormal;
	vec3 FragPosition;
	vec4 VertexColor;
	vec2 TextureCoords;
} fs_in;

out vec4 FragColor;

void main(){
		// ambient
    vec3 ambient = light.ambiant * vec3(texture(material.diffuse, fs_in.TextureCoords));

    // diffuse
    vec3 norm = normalize(fs_in.VertexNormal);
    vec3 lightDir = normalize(light.position - fs_in.FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TextureCoords));

    // specular
    vec3 viewDir = normalize(viewPosition - fs_in.FragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TextureCoords));

    vec3 result = (ambient + diffuse + specular) * fs_in.VertexColor.rgb;
    FragColor = vec4(result, fs_in.VertexColor.a);
}
