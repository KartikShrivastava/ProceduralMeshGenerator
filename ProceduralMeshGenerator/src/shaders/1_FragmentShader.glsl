#version 330 core

in vec2 t_texCoords;
in vec3 t_worldPos;
in vec3 t_normal;

uniform sampler2D u_textureWood;
uniform sampler2D u_textureYayi;
uniform vec3 u_lightPos;
uniform vec3 u_lightColor;
uniform float u_mixValue;

out vec4 o_col;

void main(){
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_lightColor;

	vec3 lightDir = normalize(u_lightPos - t_worldPos);
	vec3 diffuse = max(dot(normalize(t_normal),lightDir), 0.0) * u_lightColor;

	vec3 res = ambient + diffuse;

	o_col = mix(texture(u_textureWood, t_texCoords), texture(u_textureYayi, vec2(1.0-t_texCoords.x,t_texCoords.y)), u_mixValue) * vec4(res, 1.0);
}