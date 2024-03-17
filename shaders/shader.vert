#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(set = 0, binding = 0) uniform Camera {
    mat4 proj;
    mat4 view;
} camera;

layout(set = 1, binding = 0) uniform Mesh {
    mat4 model;
} mesh;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = camera.proj * camera.view * mesh.model * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}