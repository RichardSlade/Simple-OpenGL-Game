#version 400

in vec3 UV;

uniform samplerCube myCubeSampler;

out vec4 color;

void main()
{
    color = texture(myCubeSampler, UV);
//    color = vec4(0.f, 1.f, 1.f, 1.f);
}


