#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec4 lightDirection;

uniform vec3 SkyColor;

uniform sampler2D skyTexture;
uniform sampler2D starTexture;

void main()
{
    vec2 texCoord = vec2(TexCoord.x, TexCoord.y);

    vec4 skyColor = texture2D(skyTexture, texCoord);
    vec4 starColor = texture2D(starTexture, texCoord);

    skyColor *= SkyColor.x;

    skyColor = mix(starColor, skyColor, SkyColor.y);

    float dotproduct = dot(lightDirection.xyz, Normal);
    float angleRcp = 1 / acos(dotproduct * lightDirection.w / length(Normal));

    skyColor += angleRcp * 0.04;

    FragColor = vec4(skyColor.rgb, 1.0);
}