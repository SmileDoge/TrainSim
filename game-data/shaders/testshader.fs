#version 330 core

out vec4 FragColor;


struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform Light light;
uniform sampler2D ourTexture;
uniform vec3 viewPos;

uniform vec3 ambient;
uniform vec3 specular;
uniform float shininess;

void main()
{
    /*
    vec3 textureTexel = texture(ourTexture, vec2(TexCoord.x, TexCoord.y)).xyz;
    //vec3 textureTexel = vec3(0.8, 0.8, 0.8);

    vec3 ambient = light.ambient * textureTexel;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * textureTexel);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * (spec * specular);

    vec3 result = ambient + diffuse + specular;
    */
    vec3 result = texture(ourTexture, TexCoord).xyz;
    FragColor = vec4(result, 1.0);
}