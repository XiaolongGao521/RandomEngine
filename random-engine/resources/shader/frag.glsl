#version 330 core
in vec3 normal;
in vec3 worldPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 fragColor;

void main()
{
    vec3 lightColor = vec3(1.0);
    vec3 color = vec3(1.0, 0.5, 0.31);

    // ambient
    vec3 ambient = 0.2 * lightColor;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - worldPos);
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    vec3 specular = 0.7 * pow(max(dot(viewDir, reflectDir), 0.0), 32) * lightColor;   

    fragColor = vec4(color * (ambient + diffuse + specular), 1.0);
} 