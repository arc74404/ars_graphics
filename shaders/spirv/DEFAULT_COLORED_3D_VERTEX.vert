#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

layout(binding = 9) uniform UboBuffer {
    mat4 camera;
} ubo_buffer;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec2 vTexCoord;

void main() {
    vec3 position = aPosition;
    gl_Position = vec4(position, 1.0);
    vColor = aColor;
    vTexCoord = vec2(0.0, 0.0);          
    vNormal = vec3(0.0, 0.0, 0.0);       
}