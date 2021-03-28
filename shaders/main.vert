#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex1;
layout(location = 1) in vec3 vertex2;

out vec3 out_color;

void main(){

  gl_Position.xyz = vertex1;
  gl_Position.w = 1.0;
  
  out_color = vertex2;
}

