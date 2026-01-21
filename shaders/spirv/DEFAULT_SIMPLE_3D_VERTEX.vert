#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexCoord;
layout(location = 2) out vec3 vNormal;

void main() {
    gl_Position = vec4(position, 1.0);
    vColor = vec4(0.0, 0.0, 0.0, 0.0);   // Дефолтное значение (не используется)
    vTexCoord = aTexCoord;
    vNormal = vec3(0.0, 0.0, 0.0);       // Дефолтное значение (не используется)
}