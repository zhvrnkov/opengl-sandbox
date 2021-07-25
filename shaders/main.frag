#version 330 core

#define PI 3.1415926538

in vec2 tex_coord;

uniform float angle;
uniform sampler2D ourTexture;

out vec4 color;

void main() {
  color = texture(ourTexture, tex_coord);
}
