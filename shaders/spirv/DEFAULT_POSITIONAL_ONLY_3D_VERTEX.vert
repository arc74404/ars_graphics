#version 450 core

layout(location = 0) in vec3 aPosition;

layout(binding = 9) uniform UboBuffer {
    mat4 camera;
} ubo_buffer;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec2 vTexCoord;

void main() {
    gl_Position = ubo_buffer.camera * vec4(aPosition, 1.0);
    vColor = vec4(1.0, 1.0, 1.0, 1.0);        
    vNormal = vec3(0.0, 0.0, 1.0);
    vTexCoord = vec2(0.0, 0.0);
}