#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    FragPos = vec3(model_mat * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model_mat))) * aNormal;

    gl_Position = proj_mat * view_mat * vec4(FragPos, 1.0);
    TexCoord = aTexCoord;
}