#version 330 core

#define PI 3.1415926538
#define OBJECTS_COUNT 64

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 a_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec2 tex_coord;

void main() {
  // vec3 translation = translations[object_index];

  // vec3 rotationVector = rotation_vectors[object_index];
  // float rotationAngle = rotation_angles[object_index];

  // mat4 model = make_model(translation, rotationVector, rotationAngle);
  // mat4 view = make_view(camera_pos, camera_pos + camera_direction, up);
  // mat4 projection = make_projection_angle(45.0, 1, 0.1, 100);

  gl_Position = projection * view * model * vec4(position, 1);
  tex_coord = a_tex_coord;
}
