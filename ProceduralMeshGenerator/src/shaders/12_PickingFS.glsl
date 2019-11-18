#version 330

uniform uint u_drawIndex;
uniform uint u_objectIndex;

out vec3 o_color;

void main(){
	o_color = vec3(float(u_objectIndex), float(u_drawIndex), float(gl_PrimitiveID+1));
}