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
Scene* MapLoader::getScene(const std::string &path) {
  scene = new Scene();

  TiXmlDocument doc(Environment::getDataDir() + "/maps/" + path + ".xml");
  bool loaded = doc.LoadFile();

  if (loaded) {
    TiXmlHandle docHandle(&doc);
    TiXmlElement *element = docHandle.FirstChildElement().ToElement();
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
    cout << Environment::getDataDir() << "/" << path << ".xml" << endl;
  }
  return scene;
}

/**
 * Extract a spawn element containing its rotation and position elements
 */
void MapLoader::extractSpawn() {
  TiXmlElement *spawnElement = rootHandle.FirstChild("spawn").ToElement();

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
  TiXmlElement* lightElement = rootHandle.FirstChild("light").ToElement();

  do {
    XmlHelper::pushAttributeVertexToVector(lightElement, lightPos);

    lightElement->QueryFloatAttribute("r", &lightColor.x);
    lightElement->QueryFloatAttribute("g", &lightColor.y);
    lightElement->QueryFloatAttribute("b", &lightColor.z);

    lightElement->QueryFloatAttribute("distance", &distance);
    lightElement->QueryFloatAttribute("energy", &energy);

    scene->lights.emplace_back();
    Light &light = scene->lights.back();
    light.position.set(lightPos.x, lightPos.y, lightPos.z);
    light.color.set(lightColor.x, lightColor.y, lightColor.z);
    light.distance = distance;
    light.energy = energy;
  } while ((lightElement = lightElement->NextSiblingElement("light")) != nullptr);
}

void MapLoader::extractDoor() {
  TiXmlElement *endElement = rootHandle.FirstChild("end").ToElement();

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
  TiXmlElement *textureElement = rootHandle.FirstChild("texture").ToElement();
  string texturePath("none");
  string surfaceType("none");

  if (textureElement) {
    do {
      textureElement->QueryStringAttribute("source", &texturePath);
      textureElement->QueryStringAttribute("type", &surfaceType);
      TiXmlElement *wallBoxElement = textureElement->FirstChildElement("wall");

      if (wallBoxElement) {
        do {
          scene->walls.emplace_back();
          Entity &wall = scene->walls.back();

          TiXmlElement *boxPositionElement = wallBoxElement->FirstChildElement("position");
          XmlHelper::pushAttributeVertexToVector(boxPositionElement, wall.position);

          TiXmlElement *boxScaleElement = wallBoxElement->FirstChildElement("scale");
          XmlHelper::pushAttributeVertexToVector(boxScaleElement, wall.scale);

          wall.texture = TextureLoader::getTexture(texturePath);
          wall.texture.xTiling = 0.5f;
          wall.texture.yTiling = 0.5f;
          wall.mesh = MeshLoader::getPortalBox(wall);
        } while ((wallBoxElement = wallBoxElement->NextSiblingElement("wall")) != nullptr);
      }

      texturePath = "none";
    } while ((textureElement = textureElement->NextSiblingElement("texture")) != nullptr);
  }
}

void MapLoader::extractTriggers() {
  TiXmlElement *triggerElement = rootHandle.FirstChild("trigger").ToElement();
  string triggerType("none");

  if (triggerElement) {
    do {
      TiXmlElement *triggerTypeElement;

      scene->triggers.emplace_back();
      Trigger &trigger = scene->triggers.back();

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

    } while ((triggerElement = triggerElement->NextSiblingElement()) != nullptr);
  }
}

void MapLoader::extractModels() {
  Vector3f modelPos;
  string texture("none");
  string mesh("none");
  TiXmlElement *modelElement = rootHandle.FirstChild("model").ToElement();
  if (modelElement){
    do {
      modelElement->QueryStringAttribute("texture", &texture);
      modelElement->QueryStringAttribute("mesh", &mesh);
      XmlHelper::pushAttributeVertexToVector(modelElement, modelPos);

      scene->models.emplace_back();
      Entity &model = scene->models.back();
      XmlHelper::extractPositionAndRotation(modelElement, model);
      model.texture = TextureLoader::getTexture(texture);
      model.mesh = MeshLoader::getMesh(mesh);
    } while ((modelElement = modelElement->NextSiblingElement("model")) != nullptr);
  }
}


void MapLoader::extractButtons() {
  TiXmlElement *textureElement = rootHandle.FirstChild("texture").ToElement();
  string texturePath("none");
  string surfaceType("none");
  Vector2f position;
  Vector2f size;

  if (textureElement) {
    do {
      textureElement->QueryStringAttribute("source", &texturePath);
      textureElement->QueryStringAttribute("type", &surfaceType);
      TiXmlElement *buttonElement = textureElement->FirstChildElement("GUIbutton");

      if (buttonElement) {
        do {
          scene->buttons.emplace_back();
          GUIButton &button = scene->buttons.back();

          buttonElement->QueryFloatAttribute("x", &position.x);
          buttonElement->QueryFloatAttribute("y", &position.y);

          buttonElement->QueryFloatAttribute("w", &size.x);
          buttonElement->QueryFloatAttribute("h", &size.y);

          button.texture = TextureLoader::getTexture(texturePath);
          button.texture.xTiling = 0.5f;
          button.texture.yTiling = 0.5f;
        } while ((buttonElement = buttonElement->NextSiblingElement("GUIbutton")) != nullptr);
      }

      texturePath = "none";
    } while ((textureElement = textureElement->NextSiblingElement("texture")) != nullptr);
  }
}
} /* namespace glPortal */
