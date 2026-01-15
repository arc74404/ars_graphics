#version 450 core

layout(location = 0) in vec3 aPosition;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec2 vTexCoord;

void main() {
    vec3 position = aPosition;
    position.z += 0.2;
    gl_Position = vec4(position, 1.0);
    vColor = vec4(1.0, 1.0, 1.0, 1.0);        
    vTexCoord = vec2(0.0, 0.0);
}
