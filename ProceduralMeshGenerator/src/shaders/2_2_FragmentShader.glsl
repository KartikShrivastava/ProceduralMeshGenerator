#version 330 core

in vec3 t_worldPos;
in vec3 t_normal;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 u_camPos;
uniform Material u_material;
uniform Light u_light;

out vec4 o_color;

void main(){
	vec3 ambient = u_light.ambient * u_material.ambient;

	vec3 normal = normalize(t_normal);
	vec3 lightDir = normalize(u_light.position - t_worldPos);
	vec3 diffuse = u_light.diffuse * (max(dot(normal, lightDir), 0.0) * u_material.diffuse);

	vec3 viewDir = normalize(u_camPos - t_worldPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 specular = u_light.specular * (pow(max(dot(viewDir,reflectDir), 0.0), u_material.shininess) * u_material.specular);

	vec3 res = ambient + diffuse + specular;
	o_color = vec4(res, 1.0);
}