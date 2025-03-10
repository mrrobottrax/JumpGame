#version 450

layout(set = 0, binding = 0) uniform sampler2D atlas;
layout(set = 1, binding = 0) uniform usampler2D level;

layout(location = 0) in vec2 tilePos;

layout(location = 0) out vec4 colour;

layout(push_constant) uniform PushConstants {
    ivec2 levelSize;
    int tileSize;
} pc;

void main()
{
    ivec2 rough = ivec2(floor(tilePos));
    ivec2 fine = ivec2(floor(tilePos * pc.tileSize));

    ivec2 levelPos = ivec2(rough.x, pc.levelSize.y - rough.y - 1);
    uint tileIndex = texelFetch(level, levelPos, 0).r;

    ivec2 texCoord = fine - ivec2(rough * pc.tileSize);
    texCoord.y = pc.tileSize - texCoord.y - 1;

    ivec2 atlasSize = textureSize(atlas, 0) / pc.tileSize;
    uint atlasY = tileIndex / atlasSize.x;

    ivec2 baseCoord = ivec2(tileIndex - atlasY * atlasSize.x, atlasY) * pc.tileSize;

    colour = texelFetch(atlas, texCoord + baseCoord, 0);
    if (colour.a < 0.5) discard;
}
