#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 vUV;

out vec2 UV;

void main()
{
    gl_Position = vPosition;
    UV = vUV;
}


