#version 330 core

//	i_	denotes attributes which are passed from CPU
//	t_	denotes attributes which are transferred between shaders
//	u_	denotes uniforms
//	o_	denotes attributes which frangment shader outputs

layout (location = 0) in vec4 i_pos;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_tex;

uniform mat4 u_mvp;
uniform mat4 u_model;

out vec2 t_texCoords;
out vec3 t_normal;
out vec3 t_worldPos;

void main(){
	gl_Position = u_mvp * i_pos;
	t_texCoords = i_tex;
	t_worldPos = vec3(u_model * i_pos);
	t_normal = mat3(u_model) * i_normal;
}