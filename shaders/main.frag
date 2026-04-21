#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} fs_in;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D shadowMap;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;
uniform float shininess;

float shadowFactor(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0)
        return 0.0;

    float currentDepth = projCoords.z;
    float bias = max(0.004 * (1.0 - dot(normal, lightDir)), 0.0015);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float closestDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
        }
    }
    return shadow / 9.0;
}

void main()
{
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float dist = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0 / (
        constantAttenuation +
        linearAttenuation * dist +
        quadraticAttenuation * dist * dist
    );

    vec3 ambient = 0.05 * objectColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * objectColor;

    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    float shadow = shadowFactor(fs_in.FragPosLightSpace, normal, lightDir);
    vec3 lit = ambient + (1.0 - shadow) * attenuation * (diffuse + specular);

    FragColor = vec4(lit, 1.0);
}
