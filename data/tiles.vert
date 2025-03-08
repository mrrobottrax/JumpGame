#version 450

layout(location = 0) in vec2 aPos;

layout(push_constant) uniform PushConstants {
    vec2 scaling;
} pc;

void main()
{
    gl_Position = vec4(aPos.xy, 0, 1.0);
}
