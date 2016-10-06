#version 330 core

uniform vec4 u_colour;
out vec4 color;

void main()
{
	color = u_colour /*vec4(1.0f, 1.0f, 1.0f, 1.0f)*/;
}
