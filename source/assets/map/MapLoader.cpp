#include "MapLoader.hpp"

#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>

#include <assets/map/XmlHelper.hpp>
#include <engine/env/Environment.hpp>
#include <engine/core/math/Vector3f.hpp>

#include <engine/component/Transform.hpp>
#include <engine/component/MeshDrawable.hpp>
#include <engine/component/Trigger.hpp>
#include <engine/component/SoundSource.hpp>
#include <engine/component/LightSource.hpp>
#include <engine/component/RigidBody.hpp>
#include <engine/component/Player.hpp>

#include <assets/scene/Scene.hpp>
#include <assets/model/Mesh.hpp>
#include <assets/texture/Texture.hpp>

#include <assets/model/MeshLoader.hpp>
#include <assets/texture/TextureLoader.hpp>
#include <assets/material/MaterialLoader.hpp>

using namespace std;
using namespace tinyxml2;

namespace glPortal {
/** \class MapLoader
 *  Load a map in GlPortal XML format.
 *  @ref map-format-spec
 */

  Scene* MapLoader::scene;
  XMLHandle MapLoader::rootHandle = XMLHandle(0);

/**
 * Get a scene from a map file in XML format.
 */
Scene* MapLoader::getSceneFromPath(const std::string &path) {
  scene = new Scene();
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
          System::Log(Error) << "Invalid Material ID in map.";
          continue;
        }
        std::string name = matElm->Attribute("name");
        if (name.length() > 0) {
          scene->materials[mid] = MaterialLoader::getMaterial(name);
        } else {
          System::Log(Error) << "Name is mandatory for mat tag.";
          continue;
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
    scene->start = &scene->entities.create();
    Transform &t = scene->start->addComponent<Transform>();
    Vector3f position;
    XmlHelper::extractPosition(spawnElement, position);
    t.setPosition(position);
    Player &p = scene->player->getComponent<Player>();
    XmlHelper::extractRotation(spawnElement, p.headAngle);
    Transform &pt = scene->player->getComponent<Transform>();
    pt.setPosition(position);
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

    Entity &light = scene->entities.create();
    Transform &t = light.addComponent<Transform>();
    t.setPosition(lightPos);
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
    Entity &door = scene->entities.create();
    scene->end = &door;
    door.clearComponents();
    Transform &t = door.addComponent<Transform>();
    Vector3f position;
    XmlHelper::extractPosition(endElement, position);
    t.setPosition(position);
    Vector3f angles;
    XmlHelper::extractRotation(endElement, angles);
    t.setOrientation(Quaternion().fromAero(angles));
    MeshDrawable &m = door.addComponent<MeshDrawable>();
    m.material = MaterialLoader::loadFromXML("door/door");
    m.mesh = MeshLoader::getMesh("Door.obj");
  }
}

void MapLoader::extractWalls() {
  XMLElement *wallBoxElement = rootHandle.FirstChildElement("wall").ToElement();

  if (wallBoxElement) {
    do {
      Entity &wall = scene->entities.create();

      Transform &t = wall.addComponent<Transform>();
      Vector3f position;
      XmlHelper::extractPosition(wallBoxElement, position);
      t.setPosition(position);
      Vector3f angles;
      XmlHelper::extractRotation(wallBoxElement, angles);
      t.setOrientation(Quaternion().fromAero(angles));
      Vector3f scale;
      XmlHelper::extractScale(wallBoxElement, scale);
      t.setScale(scale);

      int mid = -1;
      wallBoxElement->QueryIntAttribute("mid", &mid);
      MeshDrawable &m = wall.addComponent<MeshDrawable>();
      m.material = scene->materials[mid];
      m.material.scaleU = m.material.scaleV = 2.f;
      m.mesh = MeshLoader::getPortalBox(wall);
      wall.addComponent<RigidBody>
        (0, std::make_shared<btBoxShape>(btVector3(t.getScale().x/2,
                                                   t.getScale().y/2, t.getScale().z/2)));
    } while ((wallBoxElement = wallBoxElement->NextSiblingElement("wall")) != nullptr);
  }
}

void MapLoader::extractAcids() {
  XMLElement *acidElement = rootHandle.FirstChildElement("acid").ToElement();
  
  if (acidElement) {
    do {
      Entity &acid = scene->entities.create();

      Transform &t = acid.addComponent<Transform>();
      Vector3f position;
      XmlHelper::extractPosition(acidElement, position);
      t.setPosition(position);
      Vector3f scale;
      XmlHelper::extractScale(acidElement, scale);
      t.setScale(scale);

      MeshDrawable &m = acid.addComponent<MeshDrawable>();
      m.material = MaterialLoader::loadFromXML("fluid/acid00");
      m.mesh = MeshLoader::getPortalBox(acid);
    } while ((acidElement = acidElement->NextSiblingElement("acid")) != nullptr);
  }
}

void MapLoader::extractTriggers() {
  XMLElement *triggerElement = rootHandle.FirstChildElement("trigger").ToElement();

  if (triggerElement) {
    do {
      Entity &trigger = scene->entities.create();

      Transform &t = trigger.addComponent<Transform>();
      Vector3f position;
      XmlHelper::extractPosition(triggerElement, position);
      t.setPosition(position);
      Vector3f scale;
      XmlHelper::extractScale(triggerElement, scale);
      t.setScale(scale);
      
      Trigger &tgr = trigger.addComponent<Trigger>();
      tgr.type = triggerElement->Attribute("type");
    } while ((triggerElement = triggerElement->NextSiblingElement("trigger")) != nullptr);
  }
}

void MapLoader::extractModels() {
  int mid = -1;
  string mesh("none");
  XMLElement *modelElement = rootHandle.FirstChildElement("object").ToElement();
  if (modelElement){
    do {
      modelElement->QueryIntAttribute("mid", &mid);
      mesh = modelElement->Attribute("mesh");

      Entity &model = scene->entities.create();
      Transform &t = model.addComponent<Transform>();
      Vector3f position;
      XmlHelper::extractPosition(modelElement, position);
      t.setPosition(position);
      Vector3f angles;
      XmlHelper::extractRotation(modelElement, angles);
      t.setOrientation(Quaternion().fromAero(angles));
      MeshDrawable &m = model.addComponent<MeshDrawable>();
      m.material = scene->materials[mid];
      m.mesh = MeshLoader::getMesh(mesh);
    } while ((modelElement = modelElement->NextSiblingElement("object")) != nullptr);
  }
}
} /* namespace glPortal */
