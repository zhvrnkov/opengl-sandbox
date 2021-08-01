#version 330 core

#define PI 3.1415926538
#define OBJECTS_COUNT 64

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 a_tex_coord;

uniform int object_index;
uniform vec3 translations[OBJECTS_COUNT];
uniform vec3 rotation_vectors[OBJECTS_COUNT];
uniform float rotation_angles[OBJECTS_COUNT];
uniform float camera_angle;

out vec2 tex_coord;

float radians(float angle) {
  return angle * 180 / PI;
}

mat4 translate(vec3 translation) {
  float x = translation.x;
  float y = translation.y;
  float z = translation.z;
  return mat4(1.0, 0.0, 0.0, 0.0,  
              0.0, 1.0, 0.0, 0.0,  
              0.0, 0.0, 1.0, 0.0,  
              x, y, z, 1.0);
}

mat4 rotate(mat4 src, float angle, vec3 vector) {
  float s = PI / 2;
  float shangle = angle + s;

  float zangle = angle * vector.z;
  vec2 zx_vec = vec2(cos(zangle), sin(zangle));
  vec2 zy_vec = vec2(cos(zangle + s), sin(zangle + s));
  mat4 z_rot = mat4(zx_vec.x, zy_vec.x, 0.0, 0.0,
                    zx_vec.y, zy_vec.y, 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0,
                    0.0, 0.0, 0.0, 1.0);

  float yangle = angle * vector.y;
  vec3 yx_vec = vec3(cos(yangle), 0, sin(yangle));
  vec3 yy_vec = vec3(cos(yangle + s), 0, sin(yangle + s));
  mat4 y_rot = mat4(yx_vec.x, 0.0, yy_vec.x, 0.0,  
                    0.0, 1.0, 0.0, 0.0,  
                    yx_vec.z, 0.0, yy_vec.z, 0.0,  
                    0.0, 0.0, 0.0, 1.0);

  float xangle = angle * vector.x;
  vec3 xy_vec = vec3(0, cos(xangle), sin(xangle));
  vec3 xz_vec = vec3(0, cos(xangle + s), sin(xangle + s));
  mat4 x_rot = mat4(1.0, 0.0, 0.0, 0.0,  
                    0.0, xy_vec.y, xz_vec.y, 0.0,  
                    0.0, xy_vec.z, xz_vec.z, 0.0,  
                    0.0, 0.0, 0.0, 1.0);
  mat4 no_t = mat4(mat3(src));
  return z_rot * x_rot * y_rot * src;
}

mat4 make_model(vec3 translation, vec3 rotationVector, float rotationAngle) {
  return rotate(translate(translation), rotationAngle, rotationVector);
}

mat4 make_view() {
  return mat4(1.0);
}

mat4 make_projection() {
  float xra = PI / 2 + PI / 16;
  float xrs = xra + PI / 2;
  mat3 projection = mat3(1, 0,          0,
                         0, sin(xra), sin(xrs),
                         0, cos(xra), cos(xrs));
  return mat4(projection);
}

void main() {
  vec3 translation = translations[object_index];

  vec3 rotationVector = rotation_vectors[object_index];
  float rotationAngle = rotation_angles[object_index];

  mat4 model = make_model(translation, rotationVector, rotationAngle);
  mat4 view = make_view();
  mat4 projection = make_projection();

  gl_Position = projection * view * model * vec4(position, 3);
  tex_coord = a_tex_coord;
}
