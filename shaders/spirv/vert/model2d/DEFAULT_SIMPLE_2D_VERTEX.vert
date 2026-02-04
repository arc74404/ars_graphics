#version 450 core

layout(location = 0) in vec2 aPosition;   
layout(location = 1) in vec2 aTexCoord;   

layout(binding = 9) uniform UboBuffer {
    mat4 camera;
} ubo_buffer;

layout(std430, binding = 15) readonly buffer ModelInstances {
    mat4 model_transforms[];
} model_instances;

layout(std430, binding = 16) readonly buffer MeshInstances {
    mat4 mesh_transforms[];
} mesh_instances;

///////////////

struct InstanceMapping {
    uint model_index;
    uint mesh_index;
};

layout(std430, binding = 17) readonly buffer MappingBuffer {
    InstanceMapping mappings[];
} mapping_buffer;


///////////////

layout(location = 0) out vec2 vTexCoord;

void main() {
    InstanceMapping mapping = mapping_buffer.mappings[gl_InstanceIndex];
    uint model_index = mapping.model_index;
    uint mesh_index = mapping.mesh_index;
    
    mat4 model_transform = model_instances.model_transforms[model_index];
    mat4 mesh_transform = mesh_instances.mesh_transforms[mesh_index];
    
    mat4 combined_transform = model_transform * mesh_transform;
    
    vec4 world_position = combined_transform * vec4(aPosition, 1.0, 1.0);
    
    gl_Position = ubo_buffer.camera * world_position;
    vTexCoord = aTexCoord;
}