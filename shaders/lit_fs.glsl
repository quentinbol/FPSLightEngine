#version 330 core
struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform int   numLights;
uniform Light lights[16];

uniform vec3 objectColor;
uniform vec3 viewPos;

uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;

in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

float shadowPCF(vec4 fragPosLightSpace, float bias) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0) return 0.0;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y)*texelSize).r;
            float currentDepth = projCoords.z - bias;
            shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}

void main() {
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 ambient = 0.15 * objectColor;
    vec3 color = vec3(0.0);

    vec3 L0 = normalize(lights[0].position - FragPos);
    float bias = max(0.001 * (1.0 - dot(N, L0)), 0.0005);

    vec4 fragLS = lightSpaceMatrix * vec4(FragPos, 1.0);
    float shadow = shadowPCF(fragLS, bias);

    for (int i = 0; i < numLights; ++i) {
        vec3 L = normalize(lights[i].position - FragPos);
        float diff = max(dot(N, L), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity;

        vec3 H = normalize(L + V);
        float shininess = 32.0;
        float specStr = pow(max(dot(N, H), 0.0), shininess);
        vec3 specular = 0.2 * specStr * lights[i].color * lights[i].intensity;

        float shade = (i == 0) ? (1.0 - shadow) : 1.0;
        color += shade * (diffuse + specular);
    }

    vec3 finalColor = ambient + color;
    finalColor *= objectColor;

    FragColor = vec4(finalColor, 1.0);
}
