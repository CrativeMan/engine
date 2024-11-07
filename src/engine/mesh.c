#include "../header/mesh.h"
#include "../header/fileHandler.h"
#include "../header/logger.h"
#include <GL/glew.h>

#define ID "Mesh"

Mesh initMesh(float *vertices, size_t verticesSize, unsigned int *indices,
              size_t indicesSize, char *textures[], size_t textureSize) {
  Mesh mesh;

  glGenVertexArrays(1, &mesh.VAO);
  glGenBuffers(1, &mesh.VBO);
  glGenBuffers(1, &mesh.EBO);

  glBindVertexArray(mesh.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
  glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

  // vertex attrib pointers
  // pointer to the position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // pointer to the color values
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // pointer to the tex coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  mesh.verticesCount = verticesSize / (5 * sizeof(float));
  mesh.indicesCount = indicesSize / sizeof(unsigned int);
  mesh.textureCount = textureSize / sizeof(textures[0]);

  mesh.textures = (unsigned int *)malloc(textureSize);
  for (int i = 0; i < mesh.textureCount; i++) {
    unsigned int texture = loadImage(textures[i]);
    mesh.textures[i] = texture;
  }

  // bind texture to shader
  for (int i = 0; i < mesh.textureCount; i++) {
    switch (i) {
    case 0:
      glActiveTexture(GL_TEXTURE0);
      break;
    case 1:
      glActiveTexture(GL_TEXTURE1);
      break;
    case 2:
      glActiveTexture(GL_TEXTURE2);
      break;
    case 3:
      glActiveTexture(GL_TEXTURE3);
      break;
    case 4:
      glActiveTexture(GL_TEXTURE4);
      break;
    case 5:
      glActiveTexture(GL_TEXTURE5);
      break;
    default:
      loggerWarn(ID, "To many textures (%d) only 5 allowed", mesh.textureCount);
    }
    glBindTexture(GL_TEXTURE_2D, mesh.textures[i]);
  }

  loggerInfo(ID, "Created mesh (VAO: %d, VBO:%d, EBO: %d, TC: %d)", mesh.VAO,
             mesh.VBO, mesh.EBO, mesh.textureCount);
  return mesh;
}

void deleteMesh(Mesh *mesh) {
  free(mesh->textures);
  loggerInfo(ID, "Deleted Mesh (TC: %d)", mesh->textureCount);
}
