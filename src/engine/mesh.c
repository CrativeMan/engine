#include <GL/glew.h>
#include <stdio.h>
#include <string.h>

#include "../header/logger.h"
#include "../header/mesh.h"
#include "GL/gl.h"
#include "GL/glext.h"

#define ID "Mesh"

int setupMesh(Mesh *self) {
  loggerInfo(ID, "Sethup mesh");
  glGenVertexArrays(1, &self->VAO);
  glGenBuffers(1, &self->VBO);
  glGenBuffers(1, &self->EBO);

  glBindVertexArray(self->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, self->VBO);

  glBufferData(GL_ARRAY_BUFFER, self->numVertices * sizeof(Vertex),
               &self->vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, self->numIndices * sizeof(unsigned int),
               &self->indices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texCoords));

  glBindVertexArray(0);

  int err = glCheckError();
  return err;
}

int drawMesh(Mesh *self, Shader *shader) {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < self->numTextures; i++) {
    glActiveTexture(GL_TEXTURE0 + i);

    char numberStr[16];
    char uniformName[64];
    char *name = self->textures[i].type;
    if (strcmp(name, "texture_diffuse") == 0) {
      snprintf(numberStr, sizeof(numberStr), "%u", diffuseNr);
      diffuseNr++;
    } else if (strcmp(name, "texture_specular") == 0) {
      snprintf(numberStr, sizeof(numberStr), "%u", specularNr);
      specularNr++;
    }
    snprintf(uniformName, sizeof(uniformName), "material.%s%s",
             self->textures[i].type, numberStr);
    shaderSetFloat(shader->id, uniformName, (float)i);
    glBindTexture(GL_TEXTURE_2D, self->textures[i].id);
  }

  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(self->VAO);
  glDrawElements(GL_TRIANGLES, self->numIndices, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  int err = glGetError();
  return err;
}
