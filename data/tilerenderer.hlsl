static const unsigned int stageWidth = 40;
static const unsigned int stageHeight = 27;
static const unsigned int tileSize = 8;

static const float2 vertices[] =
{
	float2(-1, -1),
	float2(1, -1),
	float2(-1, 1),

	float2(1, -1),
	float2(1, 1),
	float2(-1, 1),
};

static const float2 uvs[] =
{
	float2(0, 0),
	float2(1, 0),
	float2(0, 1),

	float2(1, 0),
	float2(1, 1),
	float2(0, 1),
};

Texture2DArray tiles : register(t0);
Texture2D<uint> levelTexture : register(t1);

struct VSInput
{
	uint index : SV_VertexID;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 levelPos : TEXCOORD0;
};

PSInput VSMain(VSInput input)
{
	PSInput result;

	float4 pos = float4(vertices[input.index], 0, 1);
	result.position = pos;
	result.levelPos = uvs[input.index] * uint2(stageWidth * tileSize, stageHeight * tileSize);

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	uint2 tilePosition = input.levelPos / tileSize;
	uint2 pixelPosition = (input.levelPos - tilePosition * tileSize);
	
	uint tile = levelTexture.Load(uint3(tilePosition.xy, 0));
	float3 colour = tiles.Load(uint4(pixelPosition.xy, tile, 0)).rgb;
	
	return float4(colour, 1);
}