#version 330 core

#define PI 3.1415926538

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in float angle;

uniform mat3 space;

out vec3 frag_color;

void main(){
  float shangle = angle + PI / 2;
  mat3 rspace = mat3(vec3(cos(angle), sin(angle), 0),
                     vec3(cos(shangle), sin(shangle), 0),
                     0, 0, 1);
  gl_Position = vec4(position * (space * rspace), 1.0);
  frag_color = color * rspace;
}
