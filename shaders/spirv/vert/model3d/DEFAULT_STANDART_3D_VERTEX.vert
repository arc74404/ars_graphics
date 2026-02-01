#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

layout(binding = 9) uniform UboBuffer {
    mat4 camera;
} ubo_buffer;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec2 vTexCoord;

void main() {
    gl_Position = ubo_buffer.camera * vec4(aPosition, 1.0);
    vColor = vec4(0.0, 0.0, 0.0, 0.0);  
    vNormal = aNormal;
    vTexCoord = aTexCoord;
}