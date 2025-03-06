#version 450
layout (location = 0) in vec3 aPos;

layout(push_constant) uniform PushConstants {
    vec3 position;
} pc;

void main()
{
    gl_Position = vec4(aPos + pc.position, 1.0);
}
