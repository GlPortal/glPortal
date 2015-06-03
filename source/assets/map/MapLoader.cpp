#include "MapLoader.hpp"

#include <tinyxml.h>
#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>

#include <assets/map/XmlHelper.hpp>
#include <engine/env/Environment.hpp>
#include <engine/BoxCollider.hpp>
#include <engine/volumes/Volume.hpp>
#include <engine/trigger/Trigger.hpp>
#include <engine/Light.hpp>
#include <engine/core/math/Vector3f.hpp>

#include <assets/scene/Scene.hpp>
#include <assets/model/Mesh.hpp>
#include <assets/texture/Texture.hpp>
#include <assets/gui/GUIButton.hpp>

#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>
#include <assets/material/MaterialLoader.hpp>

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

    extractMaterials();
    extractSpawn();
    extractDoor();
    extractModels();
    extractLights();
    extractWalls();
    extractAcids();
    extractTriggers();
    cout << "File loaded." << endl;
  } else {
    cout << "Unable to load file. " << endl;
    cout << Environment::getDataDir() << "/" << path << ".xml" << endl;
  }
  return scene;
}

void MapLoader::extractMaterials() {
  TiXmlElement *matIdxElm = rootHandle.FirstChild("materials").ToElement();

  if (matIdxElm) {
    TiXmlElement *matElm = matIdxElm->FirstChildElement("mat");
    if (matElm) {
      do {
        int mid = -1;
        matElm->QueryIntAttribute("mid", &mid);
        if (mid == -1) {
          continue; // Ignore, no good Material ID bound to it
        }
        std::string name("");
        matElm->QueryStringAttribute("name", &name);
        if (name.length() > 0) {
          scene->materials[mid] = MaterialLoader::getMaterial(name);
        } else {
          continue; // No name, no candy
        }
      } while ((matElm = matElm->NextSiblingElement("mat")) != nullptr);
    }
  }
}

/**
 * Extract a spawn element containing its rotation and position elements
 */
void MapLoader::extractSpawn() {
  TiXmlElement *spawnElement = rootHandle.FirstChild("spawn").ToElement();

  if (spawnElement) {
    XmlHelper::extractPosition(spawnElement, scene->start.position);
    XmlHelper::extractRotation(spawnElement, scene->start.rotation);
    scene->player.position.set(scene->start.position);
    scene->player.rotation.set(scene->start.rotation);
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
    VisualEntity door;
    XmlHelper::extractPosition(endElement, door.position);
    XmlHelper::extractRotation(endElement, door.rotation);
    door.material = MaterialLoader::fromTexture("Door.png");
    door.mesh = MeshLoader::getMesh("Door.obj");
    scene->end = door;
  }
}

void MapLoader::extractWalls() {
  TiXmlElement *wallBoxElement = rootHandle.FirstChildElement("wall").ToElement();

  if (wallBoxElement) {
    do {
      scene->walls.emplace_back();
      PhysicsEntity &wall = scene->walls.back();
      
      XmlHelper::extractPosition(wallBoxElement, wall.position);
      XmlHelper::extractRotation(wallBoxElement, wall.rotation);
      XmlHelper::extractScale(wallBoxElement, wall.scale);

      int mid = -1;
      wallBoxElement->QueryIntAttribute("mid", &mid);
      wall.material = scene->materials[mid];
      wall.material.scaleU = wall.material.scaleV = 2.f;
      wall.mesh = MeshLoader::getPortalBox(wall);
      wall.physBody = BoxCollider::generateCage(wall);
    } while ((wallBoxElement = wallBoxElement->NextSiblingElement("wall")) != nullptr);
  }
}

void MapLoader::extractAcids() {
  TiXmlElement *acidElement = rootHandle.FirstChild("acid").ToElement();
  
  if (acidElement) {
    do {
      scene->volumes.emplace_back("acid");
      Volume &acid = scene->volumes.back();
      
      acid.material.diffuse = TextureLoader::getTexture("acid.png");
      acid.mesh = MeshLoader::getPortalBox(acid);
      acid.physBody = BoxCollider::generateCage(acid);
      XmlHelper::extractPosition(acidElement, acid.position);
      XmlHelper::extractScale(acidElement, acid.scale);
    } while ((acidElement = acidElement->NextSiblingElement("acid")) != nullptr);
  }
}

void MapLoader::extractTriggers() {
  TiXmlElement *triggerElement = rootHandle.FirstChild("trigger").ToElement();

  if (triggerElement) {
    do {
      scene->triggers.emplace_back();
      Trigger &trigger = scene->triggers.back();

      if (triggerElement) {
        triggerElement->QueryStringAttribute("type", &trigger.type);
      }
      
      XmlHelper::extractPosition(triggerElement, trigger.position);
      XmlHelper::extractScale(triggerElement, trigger.scale);

    } while ((triggerElement = triggerElement->NextSiblingElement("trigger")) != nullptr);
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
      VisualEntity &model = scene->models.back();
      XmlHelper::extractPosition(modelElement, model.position);
      XmlHelper::extractRotation(modelElement, model.rotation);
      model.material = MaterialLoader::fromTexture(texture);
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

          button.material = MaterialLoader::fromTexture(texturePath);
          button.material.scaleU = button.material.scaleV = 2.f;
        } while ((buttonElement = buttonElement->NextSiblingElement("GUIbutton")) != nullptr);
      }

      texturePath = "none";
    } while ((textureElement = textureElement->NextSiblingElement("texture")) != nullptr);
  }
}
} /* namespace glPortal */
