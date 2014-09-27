#include "MapLoader.hpp"

#include <tinyxml.h>
#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>

#include "Player.hpp"
#include "Scene.hpp"
#include "engine/env/Environment.hpp"
#include "engine/Light.hpp"
#include "engine/Mesh.hpp"
#include "MeshLoader.hpp"
#include "XmlHelper.hpp"
#include "engine/Texture.hpp"
#include "TextureLoader.hpp"
#include "engine/Trigger.hpp"
#include "util/math/Vector3f.hpp"

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

  TiXmlDocument doc(string(Environment::getDataDir() + path));
  bool loaded = doc.LoadFile();

  if (loaded) {
    TiXmlHandle docHandle(&doc);
    TiXmlElement* element;
    Vector3f lightPos;
    Vector3f lightColor;
    
    element = docHandle.FirstChildElement().Element();
    rootHandle = TiXmlHandle(element);

    loadSpawn();

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

    //LIGHT
    TiXmlElement* lightElement;
    lightElement = rootHandle.FirstChild("light").Element();

    do {
      XmlHelper::pushAttributeVertexToVector(lightElement, lightPos);

      lightElement->QueryFloatAttribute("r", &lightColor.x);
      lightElement->QueryFloatAttribute("g", &lightColor.y);
      lightElement->QueryFloatAttribute("b", &lightColor.z);
      Light light;
      light.position.set(lightPos.x, lightPos.y, lightPos.z);
      light.color.set(lightColor.x, lightColor.y, lightColor.z);
      scene->lights.push_back(light);
    } while ((lightElement = lightElement->NextSiblingElement("light")) != NULL);

    //WALLS
    TiXmlElement* textureElement = rootHandle.FirstChild("texture").Element();
    string texturePath("none");
    string surfaceType("none");

    if (textureElement) {
      do {
        textureElement->QueryStringAttribute("source", &texturePath);
        textureElement->QueryStringAttribute(  "type", &surfaceType);
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

    //TRIGGER
    TiXmlElement* triggerElement = rootHandle.FirstChild("trigger").Element();
    string triggerType("none");

    if (triggerElement) {
      do {
        TiXmlElement* triggerTypeElement;
        
        Trigger trigger;
        
        if (triggerElement) {
          triggerElement->QueryStringAttribute("type", &trigger.type);
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
    cout << "File loaded." << endl;
  } else {
    cout << "Unable to load file. " << endl;
    cout << string(Environment::getDataDir()) << path << endl;
  }
  
  return scene;
}

void MapLoader::loadSpawn() {
  TiXmlElement* spawnElement;
  spawnElement = rootHandle.FirstChild("spawn").Element();

  if (spawnElement) {
    XmlHelper::extractPositionAndRotation(spawnElement, scene->player);
  } else {
    throw std::runtime_error("No spawn position defined.");
  }
}
} /* namespace glPortal */
