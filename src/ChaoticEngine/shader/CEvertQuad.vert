
#version 330 core

layout (location = 0) in vec3 VertexPosition;

uniform mat4 MVP;
uniform vec3 color;

out vec4 vertexColor;

void main(){
    vertexColor = vec4(color, 1.0);
    
    gl_Position = MVP * vec4(VertexPosition, 1.0f);
}