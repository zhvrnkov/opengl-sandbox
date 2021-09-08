#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

int main() {
  glm::mat4 m = glm::mat4(1.0f);
  m = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 2.0f, 12.0f);
  
  glm::vec4 v = glm::vec4(1, 1, -12.0f, 1);
  glm::vec4 tv = m * v;

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  std::cout << glm::to_string(projection) << std::endl;
}
