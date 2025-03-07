#version 450
layout (location = 0) in vec3 aPos;

layout(push_constant) uniform PushConstants {
    vec2 position;
    vec2 scaling;
} pc;

void main()
{
    vec2 pos = aPos.xy + pc.position;
    pos -= vec2(20, 13.5);
    pos /= vec2(20, 13.5);
    gl_Position = vec4(pos.xy, aPos.z, 1.0);
}
