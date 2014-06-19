#include <GL/glew.h>
#include "Renderer.hpp"
#include "util/Vector3f.hpp"
#include "MeshLoader.hpp"
#include "Mesh.hpp"
#include "Resource.hpp"
#include <stdio.h>

namespace glPortal {

Renderer::Renderer() : shader(Resource::loadShaders()) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glClearColor(0, 0, 0.5, 1.0);
}

void Renderer::render(Scene* scene) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shader);
  Camera camera = scene->camera;
  projectionMatrix = camera.getProjectionMatrix();

  int projLoc = glGetUniformLocation(shader, "projectionMatrix");
  int viewLoc = glGetUniformLocation(shader, "viewMatrix");
  int modelLoc = glGetUniformLocation(shader, "modelMatrix");

  //Update camera position
  viewMatrix.setIdentity();
  viewMatrix.rotate(-scene->player.rotation.x, 1, 0, 0);
  viewMatrix.rotate(-scene->player.rotation.y, 0, 1, 0);
  viewMatrix.rotate(-scene->player.rotation.z, 0, 0, 1);
  viewMatrix.translate(Vector3f::negate(scene->player.position));

  //Upload matrices
  glUniformMatrix4fv(projLoc, 1, false, projectionMatrix.array);
  glUniformMatrix4fv(viewLoc, 1, false, viewMatrix.array);

  //Lights
  int numLights = glGetUniformLocation(shader, "numLights");
  glUniform1i(numLights, scene->lights.size());

  for(unsigned int i = 0; i < scene->lights.size(); i++) {
    Light light = scene->lights[i];

    char attribute[30];
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].position");
    int lightPos = glGetUniformLocation(shader, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].color");
    int lightColor = glGetUniformLocation(shader, attribute);
    snprintf(attribute, sizeof(attribute), "%s%d%s", "lights[", i, "].intensity");
    int lightIntensity = glGetUniformLocation(shader, attribute);
    glUniform4f(lightPos, light.position.x, light.position.y, light.position.z, 1);
    glUniform3f(lightColor, light.color.x, light.color.y, light.color.z);
    glUniform1f(lightIntensity, light.intensity);
  }

  //Walls
  for(unsigned int i = 0; i < scene->walls.size(); i++) {
    Entity wall = scene->walls[i];
    modelMatrix.setIdentity();
    modelMatrix.translate(wall.position);
    modelMatrix.rotate(wall.rotation.x, 1, 0, 0);
    modelMatrix.rotate(wall.rotation.y, 0, 1, 0);
    modelMatrix.rotate(wall.rotation.z, 0, 0, 1);
    modelMatrix.scale(wall.scale);

    glUniformMatrix4fv(modelLoc, 1, false, modelMatrix.array);

    glBindVertexArray(wall.mesh.handle);

    int loc = glGetUniformLocation(shader, "diffuse");
    int tiling = glGetUniformLocation(shader, "tiling");
    glUniform2f(tiling, wall.texture.xTiling, wall.texture.yTiling);
    glUniform1i(loc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wall.texture.handle);

    glDrawArrays(GL_TRIANGLES, 0, wall.mesh.numFaces * 3);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
  }

  glUseProgram(0);
}

} /* namespace glPortal */
