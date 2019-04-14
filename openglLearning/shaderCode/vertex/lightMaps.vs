
#version 330 core
layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lampPos;

out vec3 Normal;
out vec3 FragPos;
out vec3 LampPos;
out vec2 TexCoords;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    // 将 position 转到 观察空间
    FragPos = vec3(model * vec4(position, 1.0f));
    //    Normal = normal;
    // mat3(transpose(inverse(model))) 法线矩阵 将法向量转移到世界空间
    // mat3(transpose(inverse(view * model))) 法线矩阵 将法向量转移到世界空间
    Normal = mat3(transpose(inverse(model))) * normal;
    // 将灯光位置转到观察空间
    LampPos = vec3(view * vec4(lampPos, 1.0));
    TexCoords = aTexCoords;
}
