#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;

layout(binding = 9) uniform UboBuffer {
    mat4 camera;
} ubo_buffer;

layout(std430, binding = 15) buffer ModelInstances {
    mat4 model_transforms[];
} model_instances;

layout(std430, binding = 16) buffer MeshInstances {
    mat4 mesh_transforms[];
} mesh_instances;

layout(std430, binding = 17) buffer InstanceMapping {
    uint model_indices[];
    uint mesh_indices[];
} instance_mapping;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexCoord;
layout(location = 2) out vec3 vNormal;

void main() {
    uint model_index = instance_mapping.model_indices[gl_InstanceID];
    uint mesh_index = instance_mapping.mesh_indices[gl_InstanceID];
    
    mat4 model_transform = model_instances.model_transforms[model_index];
    mat4 mesh_transform = mesh_instances.mesh_transforms[mesh_index];
    
    mat4 combined_transform = model_transform * mesh_transform;
    
    vec4 world_position = combined_transform * vec4(aPosition, 1.0);
    
    gl_Position = ubo_buffer.camera * world_position;
    
    vColor = aColor;
    vTexCoord = aTexCoord;
    vNormal = vec3(0.0, 0.0, 0.0);      
}