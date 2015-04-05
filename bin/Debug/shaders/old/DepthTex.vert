#version 330

layout (location = 0) in vec4 vPosition;

uniform mat4 MVP;

out vec2 TexCoordOut;

void main()
{
    gl_Position = MVP * vPosition;
//    TexCoordOut = vUV;
}
