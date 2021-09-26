#include "camera.h"

#define PI 3.1415926538

using namespace glm;

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
  return z_rot * x_rot * y_rot * src;
}

mat4 rotate_x(mat4 src, float angle) {
  return rotate(src, angle, vec3(1, 0, 0));
}

mat4 rotate_y(mat4 src, float angle) {
  return rotate(src, angle, vec3(0, 1, 0));
}

mat4 make_view(vec3 position, vec3 direction, vec3 up) {
  vec3 dfp = normalize(position - direction);
  vec3 newZ = dfp;
  vec3 newX = normalize(cross(up, newZ));
  vec3 newY = normalize(cross(newZ, newX));
  mat4 tr = mat4(1.0);
  tr[0] = vec4(newX.x, newY.x, newZ.x, tr[0].w);
  tr[1] = vec4(newX.y, newY.y, newZ.y, tr[1].w);
  tr[2] = vec4(newX.z, newY.z, newZ.z, tr[2].w);
  return tr * translate(-position);
}

mat4 make_projection(float w, float h, float n, float f) {
  float r = w / 2;
  float l = -w / 2;
  
  float t = h / 2;
  float b = -h / 2;

  vec4 xCol = vec4(2 * n / (r - l),
                   0,
                   0,
                   0);
  vec4 yCol = vec4(0,
                   2 * n / (t - b),
                   0,
                   0);
  vec4 zCol = vec4((r + l) / (r - l),
                   (t + b) / (t - b),
                   -(f + n) / (f - n),
                   -1);
  vec4 wCol = vec4(0,
                   0,
                   (-2 * f * n) / (f - n),
                   0);
  return mat4(xCol, yCol, zCol, wCol);
}

mat4 make_projection_angle(float fovY, float aspectRatio, float n, float f) {
  const float DEG2RAD = 3.14159265 / 180;
  float halfH = tan(fovY / 2 * DEG2RAD) * n;
  float halfW = halfH * aspectRatio;
  float w = halfW * 2;
  float h = halfH * 2;
  return make_projection(w, h, n, f);
}

mat4 make_model(vec3 translation, vec3 rotationVector, float rotationAngle) {
  mat4 rotation = rotate(mat4(1), rotationAngle, rotationVector);
  mat4 trans = translate(translation);
  return trans * rotation;
}
