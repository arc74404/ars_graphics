#version 450 core

// ParticleMesh: позиция + цвет + текстурные координаты
// Vertex attributes binding = 0
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;

// Uniform buffer binding = 9 (uniform_buffer_binding)
// Storage buffer binding = 10 (uniform_buffer_binding + 1)
// Можно добавить при необходимости:
// layout(binding = 9) uniform UniformBuffer { mat4 uModelViewProjection; };
// layout(binding = 10) readonly buffer StorageBuffer { ... };

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexCoord;
layout(location = 2) out vec3 vNormal;

void main() {
    gl_Position = ubo_buffer.camera * vec4(aPosition, 1.0);
    vColor = aColor;
    vTexCoord = aTexCoord;
    vNormal = vec3(0.0, 0.0, 0.0);       // Дефолтное значение (не используется)
}