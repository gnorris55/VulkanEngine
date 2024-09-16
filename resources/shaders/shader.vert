#version 450

layout (binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 lightPos;
    vec4 cameraPos;
    mat4 instanceTransform[17];
} ubo;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec3 fragPos;
layout(location = 4) out vec3 lightPos;
layout(location = 5) out vec3 cameraPos;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.instanceTransform[gl_InstanceIndex] * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragPos = vec3(ubo.instanceTransform[gl_InstanceIndex] * vec4(inPosition, 1));
    lightPos = ubo.lightPos.xyz;
    cameraPos = ubo.cameraPos.xyz;
    normal = mat3(transpose(inverse(ubo.instanceTransform[gl_InstanceIndex])))* inNormal;

}