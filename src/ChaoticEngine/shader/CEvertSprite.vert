
#version 330 core

layout (location = 0) in vec4 VertexPosition;

uniform mat4 model;
uniform mat4 projection;

out vec2 TexCoords;

void main(){
	TexCoords = VertexPosition.zw;

    gl_Position = projection * model * vec4(VertexPosition.xy, 0.0f, 1.0f);
}