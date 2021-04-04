#version 330 core

#define PI 3.1415926538

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 a_tex_coord;

uniform float angle;

out vec2 tex_coord;

void main(){
  float shangle = angle + PI / 2;
  mat3 space = mat3(1);
  mat3 rspace = mat3(cos(angle), cos(shangle), 0,
                     sin(angle), sin(shangle), 0,
                     0,          0,            1);
  gl_Position = vec4(position * rspace, 1.0);
  tex_coord = a_tex_coord;
}
