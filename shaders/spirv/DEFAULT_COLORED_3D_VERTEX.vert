#version 450 core

// Colored3D: позиция + цвет
// Vertex attributes binding = 0
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

// Uniform buffer binding = 9 (uniform_buffer_binding)
// Storage buffer binding = 10 (uniform_buffer_binding + 1)
// Можно добавить при необходимости:
// layout(binding = 9) uniform UniformBuffer { mat4 uModelViewProjection; };
// layout(binding = 10) readonly buffer StorageBuffer { ... };

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec2 vTexCoord;

void main() {
    // Отодвигаем кубик от камеры по оси Z
    vec3 position = aPosition;
    position.z += 2.0;  // Смещение назад (кубик размером 1x1x1, отодвигаем на 2 единицы)
    gl_Position = vec4(position, 1.0);
    vColor = aColor;
    vTexCoord = vec2(0.0, 0.0);          // Дефолтное значение (не используется)
    vNormal = vec3(0.0, 0.0, 0.0);       // Дефолтное значение (не используется)
}