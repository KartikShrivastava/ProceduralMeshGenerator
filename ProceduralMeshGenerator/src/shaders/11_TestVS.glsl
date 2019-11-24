#version 330 core

layout (location = 0) in vec4 i_pos;

uniform mat4 u_mvp;

void main()
{
    gl_Position = u_mvp * i_pos;
}