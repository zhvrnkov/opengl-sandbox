#version 330 core

#define PI 3.1415926538

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 a_tex_coord;

uniform float angle;

out vec2 tex_coord;

float radians(float angle) {
  return angle * 180 / PI;
}

mat4 rotate(mat4 src, float angle, vec3 vector) {
  return mat4(1);
}

void main() {
  float shangle = angle + PI / 2;
  mat3 rspace = mat3(cos(angle), 0, cos(shangle),
                     0, 1, 0,
                     sin(angle), 0, sin(shangle));
  float xra = PI / 2 + PI / 16;
  float xrs = xra + PI / 2;
  mat3 xrotation = mat3(1, 0,          0,
                        0, sin(xra), sin(xrs),
                        0, cos(xra), cos(xrs));
  gl_Position = vec4(position * rspace * xrotation, 1);
  tex_coord = a_tex_coord;
}
