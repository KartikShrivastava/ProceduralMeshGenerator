#version 330 core

struct Material{
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 direction;
	float innerCutoff;
	float outerCutoff;

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

out vec4 o_color;

void main(){
	vec3 ambient = u_light.ambient * texture(u_material.diffuseMap, t_texCoords).rgb;

	vec3 lightDir = normalize(u_light.position - t_worldPos);
	float theta = dot(-lightDir, normalize(u_light.direction));
	float epsilon = u_light.innerCutoff - u_light.outerCutoff;
	float intensity = clamp((theta-u_light.outerCutoff)/epsilon, 0.0, 1.0);
	
	vec3 normal = normalize(t_normal);
	vec3 diffuse = intensity * u_light.diffuse * ( max(dot(normal, lightDir),1.0) * texture(u_material.diffuseMap,t_texCoords).rgb );

	vec3 eyeDir = normalize(u_light.position - t_worldPos);
	vec3 reflectDir = reflect(-lightDir,normal);
	vec3 specular = intensity * u_light.specular * ( pow(max(dot(eyeDir,reflectDir), 0.0),u_material.shininess) * texture(u_material.specularMap, t_texCoords).rgb );

	float dist = length(u_light.position - t_worldPos);
	float attenuation = 1.0/(u_light.constant + u_light.linear*dist + u_light.quadratic*dist*dist);

	vec3 result = attenuation * (ambient + diffuse + specular);

	o_color = vec4(result,1.0);
}