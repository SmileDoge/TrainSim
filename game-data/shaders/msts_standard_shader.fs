#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 sunDirection;
uniform vec3 sunColor;

uniform vec3 worldAmbient;

uniform Light light;
uniform sampler2D ourTexture;
uniform vec3 viewPos;
uniform float shininess;

uniform float alpha_test;
uniform bool fullbright;

vec3 CalculateSunLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(sunDirection);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(sunDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient = worldAmbient * vec3(texture(ourTexture, TexCoord));
    vec3 diffuse = sunColor * diff * vec3(texture(ourTexture, TexCoord));
    vec3 specular = vec3(0.1) * spec * vec3(0.2);

    return ambient + diffuse + specular;
}

void main()
{
    vec4 textureTexel = texture(ourTexture, TexCoord);

    if (textureTexel.a <= alpha_test)
        discard;

    if (fullbright)
    {
        FragColor = textureTexel;
    }
    else
    {
        vec3 outputColor = vec3(0.0);

        vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(viewPos - FragPos);

        outputColor += CalculateSunLight(norm, viewDir);
        /*
        vec3 inColor = textureTexel.rgb;

        vec3 ambient = light.ambient * inColor;

        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = (diff * inColor);

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = light.specular * (spec * specular);

        vec3 result = ambient + diffuse + specular;
        */
        
        FragColor = vec4(outputColor, textureTexel.a);
    }
}