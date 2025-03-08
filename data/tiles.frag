#version 450

layout(binding = 0) uniform sampler2D atlas;

layout(location = 0) out vec4 colour;

void main()
{
    vec4 colourTexture = texelFetch(atlas, ivec2(0, 0), 0);
    colour = colourTexture;
}
