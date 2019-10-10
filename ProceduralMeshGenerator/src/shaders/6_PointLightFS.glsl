#version 330 core

struct Material{
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;
};

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 t_worldPos;
in vec2 t_texCoords;
in vec3 t_normal;

uniform Material u_material;
uniform Light u_light;
uniform vec3 u_camPos;

out vec4 o_color;

void main(){
	float distance = length(u_light.position - t_worldPos);
	float attenuation = 1.0/(u_light.constant + u_light.linear*distance + u_light.quadratic*distance*distance);

	vec3 ambient = attenuation * u_light.ambient * texture(u_material.diffuseMap, t_texCoords).rgb;

	vec3 normal = normalize(t_normal);
	vec3 lightDir = normalize(u_light.position - t_worldPos);
	vec3 diffuse = attenuation * u_light.diffuse * ( max(dot(normal,lightDir),0.0) * texture(u_material.diffuseMap, t_texCoords).rgb );

	vec3 eyeDir = normalize(u_camPos - t_worldPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 specular = attenuation * u_light.specular * ( pow(max(dot(eyeDir, reflectDir),0.0), u_material.shininess) * texture(u_material.specularMap, t_texCoords).rgb );

	vec3 result = ambient + diffuse + specular;
	o_color = vec4(result, 1.0);
}