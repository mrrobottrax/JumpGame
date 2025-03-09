#version 450

layout(location = 0) in vec2 aPos;

layout(location = 0) out vec2 tilePos;

layout(push_constant) uniform PushConstants {
    uvec2 levelSize;
	uint tileSize;
} pc;

void main()
{
	tilePos = aPos.xy * pc.levelSize.xy;
    gl_Position = vec4(aPos.xy * 2 - 1, 0, 1.0);
}
