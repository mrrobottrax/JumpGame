#version 450

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 position;
layout(location = 2) in uint spriteIndex;

layout(push_constant) uniform PushConstants {
    vec2 scaling;
} pc;

void main()
{
    vec2 pos = aPos.xy + position.xy;
    vec2 halfScale = pc.scaling / 2;
    pos -= halfScale;
    pos /= halfScale;
    gl_Position = vec4(pos.xy, position.z, 1.0);
}
