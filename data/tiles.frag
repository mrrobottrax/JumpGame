#version 450

layout(binding = 0) uniform sampler2D atlas;

layout(location = 0) in vec2 tilePos;
layout(location = 0) out vec4 colour;

layout(push_constant) uniform PushConstants {
    uvec2 levelSize;
	uint tileSize;
} pc;

void main()
{
    uvec2 rough = uvec2(floor(tilePos));
    uvec2 fine = uvec2(floor(tilePos * pc.tileSize));
	uvec2 texCoord = fine - rough * pc.tileSize;
	texCoord.y = pc.tileSize - texCoord.y - 1;
    vec4 colourTexture = texelFetch(atlas, ivec2(texCoord), 0);
    colour = colourTexture;
}
