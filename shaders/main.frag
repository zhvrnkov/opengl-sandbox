#version 330 core

#define PI 3.1415926538

in vec3 a_color;

uniform float angle;

out vec4 color;

void main() {
  // float shift = 2 * PI / 3;
  // float angleR = angle;
  // float angleG = angle + shift;
  // float angleB = angleG + shift;
	// vec3 tcolor = vec3(sin(angleR), sin(angleG), sin(angleB));
  color = vec4(a_color, 0);
}
