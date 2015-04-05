#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColour;

uniform mat4 MVP;

out vec4 Colour;

void main()
{
    gl_Position = MVP * vPosition;
    Colour = vColour;
}

