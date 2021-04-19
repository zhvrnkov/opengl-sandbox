#version 330 core

#define PI 3.1415926538

layout(location = 0) in vec3 position;

uniform float angle;

out vec3 a_color;

// mat3 shifted_space(float angle) {
//   float s = sin(angle);
//   float c = cos(angle);
//   return mat3(c, 1 - c, 0,
//               0, 1 - s, 0,
//               s, 1,     1);
// }

void main() {

//  float angle = PI;
  float shangle = angle + PI / 2;
  // mat3 rspace = mat3(cos(angle), cos(shangle), 0,
  //                    sin(angle), sin(shangle), 0,
  //                    0,          0,            1);
  mat3 rspace = mat3(cos(angle), 0, cos(shangle),
                     0, 1, 0,
                     sin(angle), 0, sin(shangle));

  vec3 colors[6];
  colors[0] = vec3(1, 0, 0);
  colors[1] = vec3(0, 1, 0);
  colors[2] = vec3(0, 0, 1);
  colors[3] = vec3(0.5, 0.5, 0);
  colors[4] = vec3(0.3, 0.1, 0);
  colors[5] = vec3(0, 0.8, 0.2);

  gl_Position = vec4(position * 0.5 * rspace, 1.0);
  a_color = colors[gl_InstanceID / 6];
  // a_color = colors[gl_VertexID / 6];
}
