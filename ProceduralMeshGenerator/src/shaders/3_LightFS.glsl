#version 330 core

uniform vec3 u_lightColor;

out vec4 FragColor;

void main(){
	FragColor = vec4(u_lightColor, 1.0);
}