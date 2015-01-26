#include "MapLoader.hpp"

#include <tinyxml.h>
#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>

#include <assets/map/XmlHelper.hpp>
#include <engine/env/Environment.hpp>
#include <engine/trigger/Trigger.hpp>
#include <engine/Light.hpp>
#include <engine/core/math/Vector3f.hpp>

#include <assets/scene/Scene.hpp>
#include <assets/model/Mesh.hpp>
#include <assets/texture/Texture.hpp>
#include <assets/gui/GUIButton.hpp>

#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>

#include "Player.hpp"

using namespace std;

namespace glPortal {
/** \class MapLoader
 *  Load a map in GlPortal XML format.
 */

  Scene* MapLoader::scene;
  TiXmlHandle MapLoader::rootHandle = TiXmlHandle(0);

/**
 * Get a scene from a map file in XML format.
 */
Scene* MapLoader::getScene(std::string path) {
  scene = new Scene();

  TiXmlDocument doc(string(Environment::getDataDir() + "/maps/" + path + ".xml"));
  bool loaded = doc.LoadFile();

  if (loaded) {
    TiXmlHandle docHandle(&doc);
    TiXmlElement* element;

    element = docHandle.FirstChildElement().Element();
    rootHandle = TiXmlHandle(element);

    extractSpawn();
    extractDoor();
    extractModels();
    extractLights();
    extractWalls();
    extractTriggers();
    cout << "File loaded." << endl;
  } else {
    cout << "Unable to load file. " << endl;
    cout << string(Environment::getDataDir()) << "/" << path << ".xml" << endl;
  }
  return scene;
}

/**
 * Extract a spawn element containing its rotation and position elements
 */
void MapLoader::extractSpawn() {
  TiXmlElement* spawnElement;
  spawnElement = rootHandle.FirstChild("spawn").Element();

  if (spawnElement) {
    XmlHelper::extractPositionAndRotation(spawnElement, scene->player);
  } else {
    throw std::runtime_error("No spawn position defined.");
  }
}

/**
 * Extract a light elements containing position (x, y, z) and colour (r, g, b) attributes
 */
void MapLoader::extractLights() {
  Vector3f lightPos;
  Vector3f lightColor;
  float distance;
  float energy;
  TiXmlElement* lightElement;
  lightElement = rootHandle.FirstChild("light").Element();

  do {
    XmlHelper::pushAttributeVertexToVector(lightElement, lightPos);

    lightElement->QueryFloatAttribute("r", &lightColor.x);
    lightElement->QueryFloatAttribute("g", &lightColor.y);
    lightElement->QueryFloatAttribute("b", &lightColor.z);

    lightElement->QueryFloatAttribute("distance", &distance);
    lightElement->QueryFloatAttribute("energy", &energy);

    Light light;
    light.position.set(lightPos.x, lightPos.y, lightPos.z);
    light.color.set(lightColor.x, lightColor.y, lightColor.z);
    light.distance = distance;
    light.energy = energy;
    scene->lights.push_back(light);
  } while ((lightElement = lightElement->NextSiblingElement("light")) != NULL);
}

void MapLoader::extractDoor() {
  TiXmlElement* endElement;
  endElement = rootHandle.FirstChild("end").Element();

  if (endElement) {
    Entity door;
    XmlHelper::extractPositionAndRotation(endElement, door);
    door.texture = TextureLoader::getTexture("Door.png");
    door.mesh = MeshLoader::getMesh("Door.obj");
    scene->end = door;
  } else {
    throw std::runtime_error("No end position defined.");
  }
}

void MapLoader::extractWalls() {
  TiXmlElement* textureElement = rootHandle.FirstChild("texture").Element();
  string texturePath("none");
  string surfaceType("none");

  if (textureElement) {
    do {
      textureElement->QueryStringAttribute("source", &texturePath);
      textureElement->QueryStringAttribute("type", &surfaceType);
      TiXmlElement* wallBoxElement = textureElement->FirstChildElement("wall");

      if (wallBoxElement) {
        do {
          TiXmlElement* boxPositionElement;
          TiXmlElement* boxScaleElement;

          Entity wall;
          boxPositionElement = wallBoxElement->FirstChildElement("position");
          XmlHelper::pushAttributeVertexToVector(boxPositionElement, wall.position);

          boxScaleElement = wallBoxElement->FirstChildElement("scale");
          XmlHelper::pushAttributeVertexToVector(boxScaleElement, wall.scale);

          wall.texture = TextureLoader::getTexture(texturePath);
          wall.texture.xTiling = 0.5f;
          wall.texture.yTiling = 0.5f;
          wall.mesh = MeshLoader::getPortalBox(wall);
          scene->walls.push_back(wall);
        } while ((wallBoxElement = wallBoxElement->NextSiblingElement("wall")) != NULL);
      }

      texturePath = std::string("none");
    } while ((textureElement = textureElement->NextSiblingElement("texture")) != NULL);
  }
}

void MapLoader::extractTriggers() {
  TiXmlElement* triggerElement = rootHandle.FirstChild("trigger").Element();
  string triggerType("none");

  if (triggerElement) {
    do {
      TiXmlElement* triggerTypeElement;

      Trigger trigger;

      if (triggerElement) {
        triggerElement->QueryStringAttribute("type", &trigger.type);
      }
      
      if (triggerType == "none") {
        throw std::runtime_error("Trigger must define a type attribute.");
      }

      XmlHelper::pushAttributeVertexToVector(triggerElement->FirstChildElement("position"),
                                             trigger.position);
      XmlHelper::pushAttributeVertexToVector(triggerElement->FirstChildElement("scale"),
                                             trigger.scale);
      trigger.texture = TextureLoader::getTexture("redBox.png");
      trigger.mesh = MeshLoader::getPortalBox(trigger);
      scene->triggers.push_back(trigger);

    } while ((triggerElement = triggerElement->NextSiblingElement()) != NULL);
  }
}

void MapLoader::extractModels() {
  Vector3f modelPos;
  string texture("none");
  string mesh("none");
  TiXmlElement* modelElement = rootHandle.FirstChild("model").Element();
  if (modelElement){
    do {
      modelElement->QueryStringAttribute("texture", &texture);
      modelElement->QueryStringAttribute("mesh", &mesh);
      XmlHelper::pushAttributeVertexToVector(modelElement, modelPos);

      Entity model;
      XmlHelper::extractPositionAndRotation(modelElement, model);
      model.texture = TextureLoader::getTexture(texture);
      model.mesh = MeshLoader::getMesh(mesh);
      scene->models.push_back(model);
    } while ((modelElement = modelElement->NextSiblingElement("model")) != NULL);
  }
}


void MapLoader::extractButtons() {
  TiXmlElement* textureElement = rootHandle.FirstChild("texture").Element();
  string texturePath("none");
  string surfaceType("none");
  Vector2f position;
  Vector2f size;

  if (textureElement) {
    do {
      textureElement->QueryStringAttribute("source", &texturePath);
      textureElement->QueryStringAttribute("type", &surfaceType);
      TiXmlElement* buttonElement = textureElement->FirstChildElement("GUIbutton");

      if (buttonElement) {
        do {
          GUIButton button;

          buttonElement->QueryFloatAttribute("x", &position.x);
          buttonElement->QueryFloatAttribute("y", &position.y);

          buttonElement->QueryFloatAttribute("w", &size.x);
          buttonElement->QueryFloatAttribute("h", &size.y);

          button.texture = TextureLoader::getTexture(texturePath);
          button.texture.xTiling = 0.5f;
          button.texture.yTiling = 0.5f;
          scene->buttons.push_back(button);
        } while ((buttonElement = buttonElement->NextSiblingElement("GUIbutton")) != NULL);
      }

      texturePath = std::string("none");
    } while ((textureElement = textureElement->NextSiblingElement("texture")) != NULL);
  }
}
} /* namespace glPortal */
