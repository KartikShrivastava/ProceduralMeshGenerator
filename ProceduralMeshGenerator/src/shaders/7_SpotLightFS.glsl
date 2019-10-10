#version 330 core

struct Material{
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 direction;
	float cutoff;

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
	vec3 lightDir = normalize(u_light.position - t_worldPos);
	float theta = dot(-lightDir, normalize(u_light.direction));

	vec3 ambient = u_light.ambient * texture(u_material.diffuseMap, t_texCoords).rgb;

	if(theta > u_light.cutoff){
		vec3 normal = normalize(t_normal);
		vec3 diffuse = u_light.diffuse * ( max(dot(normal, lightDir),1.0) * texture(u_material.diffuseMap,t_texCoords).rgb );

		vec3 eyeDir = normalize(u_light.position - t_worldPos);
		vec3 reflectDir = reflect(-lightDir,normal);
		vec3 specular = u_light.specular * ( pow(max(dot(eyeDir,reflectDir), 0.0),u_material.shininess) * texture(u_material.specularMap, t_texCoords).rgb );

		float distance = length(u_light.position - t_worldPos);
		float attenuation = 1.0/(u_light.constant + u_light.linear*distance + u_light.quadratic*distance*distance);

		vec3 result = attenuation * (ambient + diffuse + specular);

		o_color = vec4(result,1.0);
	}
	else{
		o_color = vec4(ambient,1.0);
	}
}