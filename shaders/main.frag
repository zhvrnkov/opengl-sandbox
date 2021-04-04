#version 330 core

#define PI 3.1415926538

in vec2 tex_coord;

out vec4 color;
uniform float angle;

uniform sampler2D ourTexture;

void main() {
  // float shift = 2 * PI / 3;
  // float angleR = angle;
  // float angleG = angle + shift;
  // float angleB = angleG + shift;
	// color = vec3(sin(angleR), sin(angleG), sin(angleB));
  color = texture(ourTexture, tex_coord);
}
