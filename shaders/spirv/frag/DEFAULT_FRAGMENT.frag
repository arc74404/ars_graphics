#version 450 core

// Входные данные из вершинного шейдера
layout(location = 0) in vec4 vColor;        // Цвет вершины (обычно не используется в PBR)
layout(location = 1) in vec3 vNormal;       // Нормаль поверхности (для освещения)
layout(location = 2) in vec2 vTexCoord;     // Текстурные координаты (UV)

// Текстуры материала
layout(set = 1, binding = 0) uniform sampler2D uAlbedoTexture;               // Базовый цвет (RGB) и прозрачность (A)
layout(set = 1, binding = 1) uniform sampler2D uNormalTexture;               // Карта нормалей
layout(set = 1, binding = 2) uniform sampler2D uMetallicRoughnessTexture;    // Металличность и шероховатость
layout(set = 1, binding = 3) uniform sampler2D uAOTexture;                   // Ambient Occlusion
layout(set = 1, binding = 4) uniform sampler2D uEmissiveTexture;             // Свечение

// Параметры материала
layout(set = 1, binding = 5) uniform MaterialParams {
    vec4 albedo_factor;      // Множитель для цвета (RGBA)
    vec4 emissive_factor;    // Цвет и сила свечения (RGB) + множитель (A)
    float metallic;          // Базовая металличность (0.0-1.0)
    float roughness;         // Базовая шероховатость (0.0-1.0)
    float ao_strength;       // Сила эффекта Ambient Occlusion (0.0-1.0)
} material;

// Выходной цвет пикселя
layout(location = 0) out vec4 FragColor;

void main() {
    // 1. Чтение текстур по одинаковым UV-координатам
    vec4 albedo = texture(uAlbedoTexture, vTexCoord);
    vec3 normal = texture(uNormalTexture, vTexCoord).rgb;
    vec3 metallicRoughness = texture(uMetallicRoughnessTexture, vTexCoord).rgb;
    float ao = texture(uAOTexture, vTexCoord).r;
    vec3 emissive = texture(uEmissiveTexture, vTexCoord).rgb;
    
    // 2. Применение параметров материала
    vec4 finalAlbedo = albedo * material.albedo_factor;
    vec3 finalEmissive = emissive * material.emissive_factor.rgb;
    
    // 3. Комбинирование текстурных и uniform-значений
    float finalMetallic = material.metallic * metallicRoughness.b;  // Metallic в синем канале
    float finalRoughness = material.roughness * metallicRoughness.g; // Roughness в зеленом канале
    float finalAO = ao * material.ao_strength;
    
    // 4. Простой вывод (только альбедо с множителем)
    FragColor = finalAlbedo;
    
    // Для отладки можно посмотреть другие каналы:
    // FragColor = vec4(vec3(finalMetallic), 1.0); // Показать металличность
    // FragColor = vec4(vec3(finalRoughness), 1.0); // Показать шероховатость
    // FragColor = vec4(normal, 1.0); // Показать нормали
    // FragColor = vec4(vec3(finalAO), 1.0); // Показать AO
    // FragColor = vec4(finalEmissive, 1.0); // Показать свечение
}