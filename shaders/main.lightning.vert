#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  float ar = 1080.0 / 1920.0;
  vec4 p = projection * view * model * vec4(aPos, 1);
  p.x = p.x * ar;
  gl_Position = p;
} 
