#version 450

layout(push_constant) uniform PushConstants {
    ivec2 levelSize;
    int tileSize;
} pc;

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 offset;
layout(location = 2) in int spriteOffset;

layout(location = 0) out vec2 texCoord;
layout(location = 1) out flat int tileIndex;

void main()
{
	bool bFlip = spriteOffset < 0;
	int flip = int(bFlip);
	int iFlip = 1 - flip;
    tileIndex = abs(spriteOffset) - flip;

    texCoord = vec2(iFlip * aPos.x + flip * (1 - aPos.x), 1 - aPos.y) * pc.tileSize;

    vec2 pos = aPos.xy + offset.xy;

    vec2 halfScale = pc.levelSize / 2.0;

    pos -= halfScale;
    pos /= halfScale;

    gl_Position = vec4(pos.xy, 0, 1.0);
}
