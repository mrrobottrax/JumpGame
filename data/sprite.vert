#version 450

layout(push_constant) uniform PushConstants {
    ivec2 levelSize;
    int tileSize;
} pc;

layout(location = 0) in vec2 aPos;

// instance
layout(location = 1) in uvec2 position;
layout(location = 2) in uvec2 dimensions;
layout(location = 3) in int spriteIndex;

layout(location = 0) out vec2 texCoord;
layout(location = 1) out flat int tileIndex;

void main()
{
    bool bFlip = spriteIndex < 0;
    int flip = int(bFlip);
    int iFlip = 1 - flip;
    tileIndex = abs(spriteIndex) - flip;

	uvec2 scale = dimensions + 1;

    texCoord = vec2(iFlip * aPos.x * scale.x + flip * (scale.x - aPos.x * scale.x), 1 - aPos.y * scale.y) * pc.tileSize;

    vec2 pos = aPos.xy * scale + position.xy / float(pc.tileSize);

    vec2 halfScale = pc.levelSize / 2.0;

    pos -= halfScale;
    pos /= halfScale;

    gl_Position = vec4(pos.xy, 0, 1.0);
}
