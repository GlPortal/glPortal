#include "MapLoader.hpp"

#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>

#include <assets/map/XmlHelper.hpp>
#include <engine/env/Environment.hpp>
#include <engine/BoxCollider.hpp>
#include <engine/core/math/Vector3f.hpp>

#include <engine/component/Transform.hpp>
#include <engine/component/MeshDrawable.hpp>
#include <engine/component/AACollisionBox.hpp>
#include <engine/component/Trigger.hpp>
#include <engine/component/Health.hpp>
#include <engine/component/SoundSource.hpp>
#include <engine/component/SoundListener.hpp>
#include <engine/component/LightSource.hpp>
#include "../../PlayerMotion.hpp"

#include <assets/scene/Scene.hpp>
#include <assets/model/Mesh.hpp>
#include <assets/texture/Texture.hpp>
#include <assets/gui/GUIButton.hpp>

#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>
#include <assets/material/MaterialLoader.hpp>

using namespace std;

namespace glPortal {
/** \class MapLoader
 *  Load a map in GlPortal XML format.
 */

  Scene* MapLoader::scene;
  XMLHandle MapLoader::rootHandle = XMLHandle(0);

/**
 * Get a scene from a map file in XML format.
 */
Scene* MapLoader::getSceneFromPath(const std::string &path) {
  scene = new Scene();
  scene->player.addComponent<Transform>();
  scene->player.addComponent<PlayerMotion>();
  scene->player.addComponent<Health>();
  scene->player.addComponent<SoundSource>();
  scene->player.addComponent<SoundListener>();
  scene->terminal.addComponent<Transform>();
  XMLDocument doc;
  XMLError error = doc.LoadFile(path.c_str());

  if (error == XML_NO_ERROR) {
    XMLHandle docHandle(&doc);
    XMLElement *element = docHandle.FirstChildElement().ToElement();
    rootHandle = XMLHandle(element);

    extractMaterials();
    extractSpawn();
    extractDoor();
    extractModels();
    extractLights();
    extractWalls();
    extractAcids();
    extractTriggers();
    System::Log(Info, "MapLoader") << "Map " << path << " loaded";
  } else {
    System::Log(Error, "MapLoader") << "Failed to load map " << Environment::getDataDir()
                       << "/" << path << ".xml";
  }
  return scene;
}
/**
 * Get a scene from a map file in XML format.
 */
Scene* MapLoader::getScene(const std::string &path) {
  return getSceneFromPath((Environment::getDataDir() + "/maps/" + path + ".xml"));
}

void MapLoader::extractMaterials() {
  XMLElement *matIdxElm = rootHandle.FirstChildElement("materials").ToElement();

  if (matIdxElm) {
    XMLElement *matElm = matIdxElm->FirstChildElement("mat");
    if (matElm) {
      do {
        int mid = -1;
        matElm->QueryIntAttribute("mid", &mid);
        if (mid == -1) {
          continue; // Ignore, no good Material ID bound to it
        }
        std::string name = matElm->Attribute("name");
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
  XMLElement *spawnElement = rootHandle.FirstChildElement("spawn").ToElement();

  if (spawnElement) {
    scene->start.clearComponents();
    Transform &t = scene->start.addComponent<Transform>();
    XmlHelper::extractPosition(spawnElement, t.position);
    XmlHelper::extractRotation(spawnElement, t.rotation);
    Transform &pt = scene->player.getComponent<Transform>();
    pt.position = t.position;
    pt.rotation = t.rotation;
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
  float distance, energy, specular;
  XMLElement* lightElement = rootHandle.FirstChildElement("light").ToElement();

  do {
    XmlHelper::pushAttributeVertexToVector(lightElement, lightPos);

    lightElement->QueryFloatAttribute("r", &lightColor.x);
    lightElement->QueryFloatAttribute("g", &lightColor.y);
    lightElement->QueryFloatAttribute("b", &lightColor.z);

    lightElement->QueryFloatAttribute("distance", &distance);
    lightElement->QueryFloatAttribute("energy", &energy);
    if (lightElement->QueryFloatAttribute("specular", &specular) == XML_NO_ATTRIBUTE) {
      specular = 0;
    }

    scene->entities.emplace_back();
    Entity &light = scene->entities.back();
    Transform &t = light.addComponent<Transform>();
    t.position = lightPos;
    LightSource &ls = light.addComponent<LightSource>();
    ls.color = lightColor;
    ls.distance = distance;
    ls.energy = energy;
    ls.specular = specular;
  } while ((lightElement = lightElement->NextSiblingElement("light")) != nullptr);
}

void MapLoader::extractDoor() {
  XMLElement *endElement = rootHandle.FirstChildElement("end").ToElement();

  if (endElement) {
    Entity &door = scene->end;
    door.clearComponents();
    Transform &t = door.addComponent<Transform>();
    XmlHelper::extractPosition(endElement, t.position);
    XmlHelper::extractRotation(endElement, t.rotation);
    MeshDrawable &m = door.addComponent<MeshDrawable>();
    m.material = MaterialLoader::loadFromXML("door/door");
    m.mesh = MeshLoader::getMesh("Door.obj");
  }
}

void MapLoader::extractWalls() {
  XMLElement *wallBoxElement = rootHandle.FirstChildElement("wall").ToElement();

  if (wallBoxElement) {
    do {
      scene->entities.emplace_back();
      Entity &wall = scene->entities.back();

      Transform &t = wall.addComponent<Transform>();
      XmlHelper::extractPosition(wallBoxElement, t.position);
      XmlHelper::extractRotation(wallBoxElement, t.rotation);
      XmlHelper::extractScale(wallBoxElement, t.scale);

      int mid = -1;
      wallBoxElement->QueryIntAttribute("mid", &mid);
      MeshDrawable &m = wall.addComponent<MeshDrawable>();
      m.material = scene->materials[mid];
      m.material.scaleU = m.material.scaleV = 2.f;
      m.mesh = MeshLoader::getPortalBox(wall);
      wall.addComponent<AACollisionBox>().box = BoxCollider::generateCage(wall);
    } while ((wallBoxElement = wallBoxElement->NextSiblingElement("wall")) != nullptr);
  }
}

void MapLoader::extractAcids() {
  XMLElement *acidElement = rootHandle.FirstChildElement("acid").ToElement();
  
  if (acidElement) {
    do {
      scene->entities.emplace_back();
      Entity &acid = scene->entities.back();

      Transform &t = acid.addComponent<Transform>();
      XmlHelper::extractPosition(acidElement, t.position);
      XmlHelper::extractScale(acidElement, t.scale);

      MeshDrawable &m = acid.addComponent<MeshDrawable>();
      m.material = MaterialLoader::loadFromXML("fluid/acid00");
      m.mesh = MeshLoader::getPortalBox(acid);
      acid.addComponent<AACollisionBox>().box = BoxCollider::generateCage(acid);
    } while ((acidElement = acidElement->NextSiblingElement("acid")) != nullptr);
  }
}

void MapLoader::extractTriggers() {
  XMLElement *triggerElement = rootHandle.FirstChildElement("trigger").ToElement();

  if (triggerElement) {
    do {
      scene->entities.emplace_back();
      Entity &trigger = scene->entities.back();

      Transform &t = trigger.addComponent<Transform>();
      XmlHelper::extractPosition(triggerElement, t.position);
      XmlHelper::extractScale(triggerElement, t.scale);
      
      Trigger &tgr = trigger.addComponent<Trigger>();
      tgr.type = triggerElement->Attribute("type");
    } while ((triggerElement = triggerElement->NextSiblingElement("trigger")) != nullptr);
  }
}

void MapLoader::extractModels() {
  Vector3f modelPos;
  string texture("none");
  string mesh("none");
  XMLElement *modelElement = rootHandle.FirstChildElement("model").ToElement();
  if (modelElement){
    do {
      texture = modelElement->Attribute("texture");
      mesh = modelElement->Attribute("mesh");
      XmlHelper::pushAttributeVertexToVector(modelElement, modelPos);

      scene->entities.emplace_back();
      Entity &model = scene->entities.back();
      Transform &t = model.addComponent<Transform>();
      XmlHelper::extractPosition(modelElement, t.position);
      XmlHelper::extractRotation(modelElement, t.rotation);
      MeshDrawable &m = model.addComponent<MeshDrawable>();
      m.material = MaterialLoader::fromTexture(texture);
      m.mesh = MeshLoader::getMesh(mesh);
    } while ((modelElement = modelElement->NextSiblingElement("model")) != nullptr);
  }
}

void MapLoader::extractButtons() {
  // FIXME
#if 0
  XMLElement *textureElement = rootHandle.FirstChild("texture").ToElement();
  string texturePath("none");
  string surfaceType("none");
  Vector2f position;
  Vector2f size;

  if (textureElement) {
    do {
      textureElement->QueryStringAttribute("source", &texturePath);
      textureElement->QueryStringAttribute("type", &surfaceType);
      XMLElement *buttonElement = textureElement->FirstChildElement("GUIbutton");

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
#endif
}
} /* namespace glPortal */
