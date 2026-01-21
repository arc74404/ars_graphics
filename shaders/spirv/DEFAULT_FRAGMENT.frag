#version 450 core

layout(location = 0) in vec4 vColor;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

// Material shader bindings (material_shader_binding = 0, set = 1):

layout(set = 1, binding = 0) uniform sampler2D uAlbedoTexture;
layout(set = 1, binding = 1) uniform sampler2D uNormalTexture;
layout(set = 1, binding = 2) uniform sampler2D uMetallicRoughnessTexture;
layout(set = 1, binding = 3) uniform sampler2D uAOTexture;
layout(set = 1, binding = 4) uniform sampler2D uEmissiveTexture;
layout(set = 1, binding = 5) uniform sampler2D uHeightTexture;

layout(set = 1, binding = 6) uniform MaterialParams {
    vec4 albedo_factor;
} material;

layout(location = 0) out vec4 FragColor;

void main() {
    vec4 albedo = texture(uAlbedoTexture, vTexCoord);
    FragColor = albedo * material.albedo_factor;
}
