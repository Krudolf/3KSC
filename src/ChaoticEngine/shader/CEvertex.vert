
#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 TextureCoords;

uniform mat4 MVP;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoords;

void main(){
    Position = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0f));
    Normal = normalize(NormalMatrix * VertexNormal);
    TexCoords = TextureCoords;

    gl_Position = MVP * vec4(VertexPosition, 1.0f);
}