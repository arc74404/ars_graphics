#version 450 core

// Simple2D: позиция + текстурные координаты
// Vertex attributes binding = 0
layout(location = 0) in vec2 aPosition;   
layout(location = 1) in vec2 aTexCoord;   

// Uniform buffer binding = 9 (uniform_buffer_binding)
// Можно добавить при необходимости:
// layout(binding = 9) uniform UniformBuffer { mat3 uTransform; mat4 uModelViewProjection; };
// layout(binding = 10) readonly buffer StorageBuffer { ... };

layout(location = 0) out vec2 vTexCoord;

void main() {
    gl_Position = vec4(aPosition, 0.0, 1.0);  // z=0 для 2D
    vTexCoord = aTexCoord;
}