#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

layout(binding = 9) uniform UboBuffer {
    mat4 camera;
} ubo_buffer;

// Model instancing 
layout(std430, binding = 10) readonly buffer ModelInstances {
    mat4 model_transforms[];
} model_instances;

// Mesh instancing into models
layout(std430, binding = 11) readonly buffer MeshInstances {
    mat4 mesh_transforms[];
} mesh_instances;

///////////////

// Index mapping

struct InstanceMapping {
    uint model_index;
    uint mesh_index; 
};

layout(std430, binding = 12) readonly buffer MappingBuffer {
    InstanceMapping mappings[];
} mapping_buffer;


///////////////

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec2 vTexCoord;

void main() {
    InstanceMapping mapping = mapping_buffer.mappings[gl_InstanceIndex];
    uint model_index = mapping.model_index;
    uint mesh_index = mapping.mesh_index;
    
    mat4 model_transform = model_instances.model_transforms[model_index];
    mat4 mesh_transform = mesh_instances.mesh_transforms[mesh_index];
    
    mat4 combined_transform = model_transform * mesh_transform;
    
    vec4 world_position = combined_transform * vec4(aPosition, 1.0);
    
    gl_Position = ubo_buffer.camera * world_position;

    vColor = aColor;
    vTexCoord = vec2(0.0, 0.0);          
    vNormal = vec3(0.0, 0.0, 0.0);       
}