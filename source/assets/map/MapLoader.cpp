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
#include <engine/component/SoundSource.hpp>
#include <engine/component/LightSource.hpp>
#include <engine/component/RigidBody.hpp>
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


Scene* MapLoader::getSceneFromPath(const std::string &path) {
  scene = new Scene;

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
    System::Log(Info) << "Map " << path << " loaded";
  } else {
    System::Log(Error) << "Failed to load map " << Environment::getDataDir()
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
    scene->start = &scene->entities.create();
    Transform &t = scene->start->addComponent<Transform>();
    Vector3f position;
    XmlHelper::extractPosition(spawnElement, position);
    t.setPosition(position);
    PlayerMotion &pm = scene->player->getComponent<PlayerMotion>();
    XmlHelper::extractRotation(spawnElement, pm.rotation);
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
    Vector3f eulerOrient;
    XmlHelper::extractRotation(endElement, eulerOrient);
    t.setOrientation(Quaternion().setFromEuler(eulerOrient));
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
      Vector3f eulerOrient;
      XmlHelper::extractRotation(wallBoxElement, eulerOrient);
      t.setOrientation(Quaternion().setFromEuler(eulerOrient));
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
        (0, std::make_shared<btBoxShape>(btVector3(t.getScale().x/2, t.getScale().y/2, t.getScale().z/2)));
      wall.addComponent<AACollisionBox>().box = BoxCollider::generateCage(wall);
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
      acid.addComponent<AACollisionBox>().box = BoxCollider::generateCage(acid);
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
  Vector3f modelPos;
  string texture("none");
  string mesh("none");
  XMLElement *modelElement = rootHandle.FirstChildElement("model").ToElement();
  if (modelElement){
    do {
      texture = modelElement->Attribute("texture");
      mesh = modelElement->Attribute("mesh");
      XmlHelper::pushAttributeVertexToVector(modelElement, modelPos);

      Entity &model = scene->entities.create();
      Transform &t = model.addComponent<Transform>();
      Vector3f position;
      XmlHelper::extractPosition(modelElement, position);
      t.setPosition(position);
      Vector3f eulerOrient;
      XmlHelper::extractRotation(modelElement, eulerOrient);
      t.setOrientation(Quaternion().setFromEuler(eulerOrient));
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
