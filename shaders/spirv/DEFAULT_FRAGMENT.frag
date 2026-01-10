#version 450 core

// Универсальный фрагментный шейдер для всех типов примитивов
// Вершинные шейдеры передают все переменные (с дефолтными значениями для отсутствующих)

layout(location = 0) in vec4 vColor;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

// Material shader bindings (material_shader_binding = 0, set = 1):

// Binding 0: Albedo map (material_shader_binding = 0)
layout(set = 1, binding = 0) uniform sampler2D uAlbedoTexture;

// Binding 1: Normal map (material_shader_binding + 1)
layout(set = 1, binding = 1) uniform sampler2D uNormalTexture;

// Binding 2: Metallic-Roughness map (material_shader_binding + 2)
layout(set = 1, binding = 2) uniform sampler2D uMetallicRoughnessTexture;

// Binding 3: AO map (material_shader_binding + 3)
layout(set = 1, binding = 3) uniform sampler2D uAOTexture;

// Binding 4: Emissive map (material_shader_binding + 4)
layout(set = 1, binding = 4) uniform sampler2D uEmissiveTexture;

// Binding 5: Height map (material_shader_binding + 5)
layout(set = 1, binding = 5) uniform sampler2D uHeightTexture;

// Uniform buffer для флагов (можно использовать вместо push constant, если нужно)
// Если используется push constant, его нужно объявить в pipeline layout с VK_SHADER_STAGE_FRAGMENT_BIT
layout(set = 1, binding = 6) uniform Uniforms {
    bool uHasColor;              // true для Colored3D и ParticleMesh
    bool uHasTexCoord;           // true для Simple3D, Standart3D и ParticleMesh
    bool uUseAlbedoTexture;      // true если нужно использовать Albedo текстуру
    bool uUseNormalTexture;      // true если есть Normal map
    bool uUseMetallicRoughnessTexture; // true если есть Metallic-Roughness map
    bool uUseAOTexture;          // true если есть AO map
    bool uUseEmissiveTexture;    // true если есть Emissive map
    bool uUseHeightTexture;      // true если есть Height map
} ubo;

layout(location = 0) out vec4 FragColor;

void main() {
    vec4 albedo = vec4(1.0, 1.0, 1.0, 1.0);  // Дефолтный белый цвет
    
    // Если есть вершинный цвет (Colored3D, ParticleMesh)
    if (ubo.uHasColor) {
        albedo = vColor;
    }
    
    // Проверка и использование Albedo текстуры
    if (ubo.uHasTexCoord && ubo.uUseAlbedoTexture) {
        vec4 texColor = texture(uAlbedoTexture, vTexCoord);
        if (ubo.uHasColor) {
            // Если есть и цвет, и текстура (ParticleMesh) - умножаем
            albedo = albedo * texColor;
        } else {
            // Если только текстура (Simple3D, Standart3D) - используем только её
            albedo = texColor;
        }
    }
    
    // Использование Normal map (для расчета освещения)
    vec3 normal = normalize(vNormal);
    if (ubo.uHasTexCoord && ubo.uUseNormalTexture) {
        // Normal map обычно в пространстве касательных, здесь упрощенная версия
        vec3 normalMapSample = texture(uNormalTexture, vTexCoord).rgb * 2.0 - 1.0;
        // В реальном PBR нужно преобразование через TBN матрицу
        normal = normalize(normal + normalMapSample * 0.5); // Упрощенное смешивание
    }
    
    // Использование Metallic-Roughness map (для PBR)
    float metallic = 0.0;
    float roughness = 0.5;
    if (ubo.uHasTexCoord && ubo.uUseMetallicRoughnessTexture) {
        vec4 mrSample = texture(uMetallicRoughnessTexture, vTexCoord);
        metallic = mrSample.b;      // Blue channel = Metallic
        roughness = mrSample.g;     // Green channel = Roughness
    }
    
    // Использование AO map (Ambient Occlusion)
    float ao = 1.0;
    if (ubo.uHasTexCoord && ubo.uUseAOTexture) {
        ao = texture(uAOTexture, vTexCoord).r;
    }
    
    // Использование Emissive map (самосвечение)
    vec3 emissive = vec3(0.0);
    if (ubo.uHasTexCoord && ubo.uUseEmissiveTexture) {
        emissive = texture(uEmissiveTexture, vTexCoord).rgb;
    }
    
    // Использование Height map (параллакс-маппинг)
    vec2 texCoord = vTexCoord;
    if (ubo.uHasTexCoord && ubo.uUseHeightTexture) {
        float height = texture(uHeightTexture, vTexCoord).r;
        // Упрощенный параллакс-маппинг (нужен view direction для полного эффекта)
        texCoord = vTexCoord + (height - 0.5) * 0.05; // Простое смещение
    }
    
    // Финальный цвет (упрощенный вариант, в реальном PBR нужны расчеты освещения)
    vec4 result = albedo * ao;
    result.rgb += emissive; // Добавляем самосвечение
    
    FragColor = result;
}
