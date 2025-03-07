#version 450

layout (location = 0) in vec2 aPos;

layout (binding = 0) uniform ObjectData {
    vec3 position;
    uint spriteIndex;
} obj;

layout(push_constant) uniform PushConstants {
    vec2 scaling;
} pc;

void main()
{
    vec2 pos = aPos.xy + obj.position.xy;
    vec2 halfScale = pc.scaling / 2;
    pos -= halfScale;
    pos /= halfScale;
    gl_Position = vec4(pos.xy, obj.position.z, 1.0);
}
