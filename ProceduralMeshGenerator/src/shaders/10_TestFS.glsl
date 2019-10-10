#version 330 core

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;

	float shininess;
};

struct DirectionalLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float innerCutoff;
	float outerCutoff;
};

in vec2 t_texCoords;
in vec3 t_worldPos;
in vec3 t_normal;

uniform Material u_material;
uniform DirectionalLight u_dLight;
#define NUM_POINT_LIGHTS 4
uniform PointLight[NUM_POINT_LIGHTS] u_pLight;
uniform SpotLight u_sLight;
uniform vec3 u_camPos;

out vec4 o_FragColor;

vec3 CalculateDirectionalLight(vec3 normal);
vec3 CalculatePointLight(vec3 normal, PointLight ponLight);
vec3 CalculateSpotLight(vec3 normal);

void main(){
	vec3 normal = normalize(t_normal);
	vec3 result = CalculateDirectionalLight(normal);
	for(int i=0; i<NUM_POINT_LIGHTS; ++i){
		result += CalculatePointLight(normal, u_pLight[i]);
	}
	result += CalculateSpotLight(normal);
	o_FragColor = vec4(result, 1.0);
}

vec3 CalculateDirectionalLight(vec3 normal){
	vec3 ambient = u_dLight.ambient * texture(u_material.texture_diffuse1, t_texCoords).rgb;

	vec3 lightDir = normalize(-u_dLight.direction);
	vec3 diffuse = u_dLight.diffuse * ( max(dot(lightDir, normal), 0.0) * texture(u_material.texture_diffuse1, t_texCoords).rgb );

	vec3 eyeDir = normalize(u_camPos - t_worldPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 specular = u_dLight.specular * ( pow( max(dot(eyeDir, reflectDir), 0.0), u_material.shininess) * texture(u_material.texture_specular1, t_texCoords).rgb );

	return ambient + diffuse;
}

vec3 CalculatePointLight(vec3 normal, PointLight ponLight){
	vec3 ambient = ponLight.ambient * texture(u_material.texture_diffuse1, t_texCoords).rgb;
	
	vec3 lightDir = normalize(ponLight.position - t_worldPos);
	vec3 diffuse = ponLight.diffuse * ( max(dot(lightDir, normal), 0.0) * texture(u_material.texture_diffuse1, t_texCoords).rgb );
	
	vec3 eyeDir = normalize(u_camPos - t_worldPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 specular = ponLight.specular * ( pow( max(dot(eyeDir, reflectDir), 0.0), u_material.shininess) * texture(u_material.texture_specular1, t_texCoords).rgb );

	float dist = length(ponLight.position - t_worldPos);
	float attenuation = 1.0 / ( ponLight.constant + ponLight.linear * dist + ponLight.quadratic * dist * dist );

	vec3 result = attenuation * (ambient + diffuse);

	return result;
}

vec3 CalculateSpotLight(vec3 normal){
	vec3 lightDir = normalize(u_camPos - t_worldPos);	//u_camPos is same as of spot light position

	float epsilon = u_sLight.innerCutoff - u_sLight.outerCutoff;
	float theta = dot(normalize(u_sLight.direction), -lightDir);
	float intensity = clamp((theta - u_sLight.outerCutoff) / epsilon, 0.0, 1.0);
	
	vec3 eyeDir = normalize(u_camPos - t_worldPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	vec3 ambient = u_sLight.ambient * texture(u_material.texture_diffuse1, t_texCoords).rgb;
	vec3 diffuse = intensity * u_sLight.diffuse * ( max(dot(lightDir, normal), 0.0) * texture(u_material.texture_diffuse1, t_texCoords).rgb );
	vec3 specular = intensity * u_sLight.specular * ( pow(max(dot(eyeDir,reflectDir), 0.0),u_material.shininess) * texture(u_material.texture_specular1, t_texCoords).rgb );

	float dist = length(u_camPos - t_worldPos);
	float attenuation = 1.0 / ( u_sLight.constant + u_sLight.linear * dist + u_sLight.quadratic * dist * dist );

	vec3 result = attenuation * (ambient + diffuse);
	
	return result;
}