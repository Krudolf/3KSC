
#version 330 core

uniform sampler2D sprite;

in vec2 TexCoords;
in vec4 ParticleColor;

out vec4 color;

void main(){
    color = (texture(sprite, TexCoords) * ParticleColor);
}  