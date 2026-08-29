#version 330 core

in vec2 textPos;
in vec3 Normal;
in vec3 FragPos;
struct Material {
    sampler2D diffusionMap;
    sampler2D specularMap;
    float shininess;
};

struct directionLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform directionLight dirLight;

struct pointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform mat4 view;
#define NR_POINT_LIGHTS 128
uniform int numOfPointLights;
uniform pointLight light[NR_POINT_LIGHTS];

out vec4 fragColor;
uniform Material material;

vec3 calculateDirectionLight(directionLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(pointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);

    vec3 resault = calculateDirectionLight(dirLight, normal, viewDir);
    for (int i = 0; i < numOfPointLights; i++) {
        resault += calculatePointLight(light[i], normal, viewDir, FragPos);
    }

    fragColor = vec4(resault, 1.0);
}

vec3 calculateDirectionLight(directionLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * texture(material.diffusionMap, textPos).rgb;
    vec3 diffusion = light.diffuse * diff * texture(material.diffusionMap, textPos).rgb;
    vec3 specular = light.specular * spec * texture(material.specularMap, textPos).rgb;

    return (ambient + diffusion + specular);
}

vec3 calculatePointLight(pointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    vec3 viewLightDir = vec3(view * vec4(light.position, 1.0));
    vec3 lightDir = normalize(viewLightDir - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    // specular

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * texture(material.diffusionMap, textPos).rgb;
    vec3 diffusion = light.diffuse * diff * texture(material.diffusionMap, textPos).rgb;
    vec3 specular = light.specular * spec * texture(material.specularMap, textPos).rgb;

    // attenuation
    float distance = length(viewLightDir - FragPos);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) +
                               (light.quadratic * distance * distance));

    ambient *= attenuation;
    diffusion *= attenuation;
    specular *= attenuation;
    return (ambient + diffusion + specular);
}
