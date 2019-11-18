#version 330 core

layout (location = 0) in vec3 i_pos;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_tex;

uniform mat4 u_mvp;
uniform vec3 u_scale;

void main(){
	gl_Position = u_mvp * vec4((i_pos * u_scale), 1.0);
}