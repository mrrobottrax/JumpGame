#version 450

layout(binding = 0) uniform sampler2D tilesTex;

layout(location = 0) out vec4 colour;

void main()
{
    colour = vec4(1, 1, 0, 1);
}
