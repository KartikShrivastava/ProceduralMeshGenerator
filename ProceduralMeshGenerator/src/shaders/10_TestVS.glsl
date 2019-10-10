#version 330 core

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_texCoords;

uniform mat4 u_mvp;
uniform mat4 u_model;

out vec2 t_texCoords;
out vec3 t_worldPos;
out vec3 t_normal;

void main(){
	gl_Position = u_mvp * i_pos;
	t_texCoords = i_texCoords;
	t_worldPos = vec3(u_model * i_pos);
	t_normal = mat3(u_model) * i_normal;
}