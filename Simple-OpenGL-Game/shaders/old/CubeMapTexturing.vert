#version 400

layout(location = 0) in vec3 vPosition;

uniform mat4 MVP;

out vec3 UV;

void main()
{
    gl_Position = MVP * vec4(vPosition, 1);
    UV = vPosition;
}
