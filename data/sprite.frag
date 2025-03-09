#version 450

layout(binding = 0) uniform sampler2D atlas;

layout(location = 0) in vec2 texCoord;
layout(location = 1) in flat uint tileIndex;

layout(location = 0) out vec4 colour;

void main()
{
    ivec2 pixelIndex = ivec2(floor(texCoord));

    colour = texelFetch(atlas, pixelIndex, 0);
    if (colour.a < 0.5) discard;
}
