#version 330 core

//using 1_VertexShader.glsl

struct Material{
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shine;
};

struct Light{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 t_texCoords;
in vec3 t_worldPos;
in vec3 t_normal;

uniform Material u_material;
uniform Light u_light;
uniform vec3 u_camPos;

out vec4 o_color;

void main(){
	vec3 ambient = u_light.ambient * texture(u_material.diffuseMap, t_texCoords).rgb;

	vec3 normal = normalize(t_normal);
	vec3 lightDir = normalize(-u_light.direction);
	vec3 diffuse = u_light.diffuse * ( max(dot(lightDir, normal), 0.0) * texture(u_material.diffuseMap, t_texCoords).rgb );

	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 eyeDir = normalize(u_camPos - t_worldPos);
	vec3 specular = u_light.specular * ( pow(max(dot(reflectDir, eyeDir), 0.0), u_material.shine) * texture(u_material.specularMap,t_texCoords).rgb );

	vec3 result = ambient + diffuse + specular;
	o_color = vec4(result, 1.0);
}