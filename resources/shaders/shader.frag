#version 450

layout (binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 fragPos;
layout(location = 4) in vec3 lightPos;
layout(location = 5) in vec3 cameraPos;

layout(location = 0) out vec4 outColor;

void main() {
   
    vec3 lightColor = vec3(1, 1, 1);

    // lighting calculations
    //ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff *lightColor;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) *  texture(texSampler, fragTexCoord).rgb;

    outColor = vec4(result, 1.0);
}