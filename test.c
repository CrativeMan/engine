#include <cglm/cglm.h>

int main() {
  // vector x=1, y=0, z=0, w=1
  vec4 vec = {1.0f, 0.0f, 0.0f, 1.0f};
  // matrix
  /*
    1 0 0 0
    0 1 0 0
    0 0 1 0
    0 0 0 1
  */
  mat4 trans;
  glm_mat4_identity(trans);
  /*
    1 0 0 1
    0 1 0 1
    0 0 1 0
    0 0 0 1
  */
  vec3 axis = {0.0f, 0.f, 1.0f};
  vec3 scale = {0.5f, 0.5f, 0.5f};
  glm_rotate(trans, glm_rad(90.0f), axis);
  glm_scale(trans, scale);
  return 0;
}
