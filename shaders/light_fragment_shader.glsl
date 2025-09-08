#version 330 core
struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform int numLights;
uniform Light lights[16];

uniform vec3 objectColor;
uniform vec3 viewPos;

in vec3 FragPos;  
in vec3 Normal;  

out vec4 FragColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);
    vec3 ambient = 0.2 * objectColor;

    float shininess = 1.0;

    for (int i = 0; i < numLights; i++) {
        vec3 lightDir = normalize(lights[i].position - FragPos);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = 0.3 * spec * lights[i].color * lights[i].intensity;

        result += diffuse + specular;
    }

    result = result * objectColor + ambient;
    FragColor = vec4(result, 1.0);
}
