#version 330 core

#define PI 3.1415926538

out vec3 color;
uniform float angle;

void main() {
  float shift = 2 * PI / 3;
  float angleR = angle;
  float angleG = angle + shift;
  float angleB = angleG + shift;
	color = vec3(sin(angleR), sin(angleG), sin(angleB));
}
