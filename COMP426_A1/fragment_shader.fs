#version 330 core

// Draws all vertices with a specified uniform colour

uniform vec4 u_colour;
out vec4 color;

void main()
{
	color = u_colour;
}
