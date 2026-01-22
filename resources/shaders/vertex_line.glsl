#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec4 colors;

out vec4 OutColor;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(vertex, 0.0, 1.0);
    OutColor = colors;
}