#version 450

layout(push_constant) uniform PushConstants {
    ivec2 levelSize;
    int tileSize;
} pc;

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 offset;
layout(location = 2) in uint spriteOffset;

layout(location = 0) out vec2 texCoord;
layout(location = 1) out flat uint tileIndex;

void main()
{
    tileIndex = spriteOffset;

    texCoord = vec2(aPos.x, 1 - aPos.y) * pc.tileSize;

    vec2 pos = aPos.xy + offset.xy;
    vec2 halfScale = pc.levelSize / 2;
    pos -= halfScale;
    pos /= halfScale;
    gl_Position = vec4(pos.xy, 0, 1.0);
}
