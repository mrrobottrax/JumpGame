#version 450

layout(push_constant) uniform PushConstants {
    ivec2 levelSize;
    int tileSize;
} pc;

layout(binding = 0) uniform sampler2D atlas;

layout(location = 0) in vec2 texCoord;
layout(location = 1) in flat uint tileIndex;

layout(location = 0) out vec4 colour;

void main()
{
    ivec2 pixelIndex = ivec2(floor(texCoord));

    ivec2 atlasSize = textureSize(atlas, 0) / pc.tileSize;
    uint atlasY = tileIndex / atlasSize.x;

    ivec2 baseCoord = ivec2(tileIndex - atlasY * atlasSize.x, atlasY) * pc.tileSize;

    colour = texelFetch(atlas, pixelIndex + baseCoord, 0);
    if (colour.a < 0.5) discard;
}
