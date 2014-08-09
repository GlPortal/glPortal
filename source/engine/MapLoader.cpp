#include "MapLoader.hpp"

#include <tinyxml.h>
#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>

#include "../Player.hpp"
#include "../Scene.hpp"
#include "environment/Environment.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "MeshLoader.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"
#include "Trigger.hpp"
#include "util/Vector3f.hpp"

using namespace std;

namespace glPortal {

Scene* MapLoader::getScene(std::string path) {
  Scene* scene = new Scene();
  TiXmlDocument doc(string(Environment::getDataDir() + path));
  bool loaded = doc.LoadFile();
 
  if(loaded) {
    TiXmlHandle docHandle(&doc);
    TiXmlElement* element;
    TiXmlHandle rootHandle(0);
    Vector3f lightPos;
    Vector3f lightColor;
    
    element = docHandle.FirstChildElement().Element();
    rootHandle = TiXmlHandle(element);
    //SPAWN
    TiXmlElement* spawnElement;
    spawnElement = rootHandle.FirstChild( "spawn" ).Element();

    if(spawnElement) {
      Vector3f spawnPos;
      Vector3f spawnRot;
      TiXmlElement* spawnPositionElement = spawnElement->FirstChildElement("position");
      spawnPositionElement->QueryFloatAttribute("x", &spawnPos.x);
      spawnPositionElement->QueryFloatAttribute("y", &spawnPos.y);
      spawnPositionElement->QueryFloatAttribute("z", &spawnPos.z);

      TiXmlElement* spawnRotationElement = spawnElement->FirstChildElement("rotation");
      spawnRotationElement->QueryFloatAttribute("x", &spawnRot.x);
      spawnRotationElement->QueryFloatAttribute("y", &spawnRot.y);
      spawnRotationElement->QueryFloatAttribute("z", &spawnRot.z);

      scene->player.position.set(spawnPos);
      scene->player.rotation.set(spawnRot);
    } else {
      throw std::runtime_error("No spawn position defined.");
    }

    //END_SPAWN
    TiXmlElement* endElement;
    endElement = rootHandle.FirstChild( "end" ).Element();

    if(endElement) {
      Vector3f endPos;
      Vector3f endRot;
      TiXmlElement* endPositionElement = endElement->FirstChildElement("position");
      endPositionElement->QueryFloatAttribute("x", &endPos.x);
      endPositionElement->QueryFloatAttribute("y", &endPos.y);
      endPositionElement->QueryFloatAttribute("z", &endPos.z);

      TiXmlElement* endRotationElement = endElement->FirstChildElement("rotation");
      endRotationElement->QueryFloatAttribute("x", &endRot.x);
      endRotationElement->QueryFloatAttribute("y", &endRot.y);
      endRotationElement->QueryFloatAttribute("z", &endRot.z);

      Entity door;
      door.position.set(endPos);
      door.rotation.set(endRot);
      door.texture = TextureLoader::getTexture("/textures/Door.png");
      door.mesh = MeshLoader::getMesh("/meshes/Door.obj");
      scene->end = door;
    } else {
      throw std::runtime_error("No end position defined.");
    }

    //LIGHT
    TiXmlElement* lightElement;
    lightElement = rootHandle.FirstChild("light").Element();

    do {
      lightElement->QueryFloatAttribute("x", &lightPos.x);
      lightElement->QueryFloatAttribute("y", &lightPos.y);
      lightElement->QueryFloatAttribute("z", &lightPos.z);

      lightElement->QueryFloatAttribute("r", &lightColor.x);
      lightElement->QueryFloatAttribute("g", &lightColor.y);
      lightElement->QueryFloatAttribute("b", &lightColor.z);
      Light light;
      light.position.set(lightPos.x, lightPos.y, lightPos.z);
      light.color.set(lightColor.x, lightColor.y, lightColor.z);
      scene->lights.push_back(light);
    } while((lightElement = lightElement->NextSiblingElement("light")) != NULL);

    //WALLS
    TiXmlElement* textureElement = rootHandle.FirstChild("texture").Element();
    string texturePath("none");
    string surfaceType("none");

    if(textureElement) {
      do {
        textureElement->QueryStringAttribute("source", &texturePath);
        textureElement->QueryStringAttribute("type"  , &surfaceType);
        TiXmlElement* wallBoxElement = textureElement->FirstChildElement("wall");

        if(wallBoxElement) {
          do {
            TiXmlElement* boxPositionElement;
            TiXmlElement* boxScaleElement;

            Entity wall;
            boxPositionElement = wallBoxElement->FirstChildElement("position");
            boxPositionElement->QueryFloatAttribute("x", &wall.position.x);
            boxPositionElement->QueryFloatAttribute("y", &wall.position.y);
            boxPositionElement->QueryFloatAttribute("z", &wall.position.z);

            boxScaleElement = wallBoxElement->FirstChildElement("scale");
            boxScaleElement->QueryFloatAttribute("x", &wall.scale.x);
            boxScaleElement->QueryFloatAttribute("y", &wall.scale.y);
            boxScaleElement->QueryFloatAttribute("z", &wall.scale.z);

            wall.texture = TextureLoader::getTexture("/textures/" + texturePath);
            wall.texture.xTiling = 0.5f;
            wall.texture.yTiling = 0.5f;
            wall.mesh = MeshLoader::getPortalBox(wall);
            scene->walls.push_back(wall);
          } while((wallBoxElement = wallBoxElement->NextSiblingElement("wall")) != NULL);
        }

        texturePath = std::string("none");
      } while((textureElement = textureElement->NextSiblingElement("texture")) != NULL);
    }

    //TRIGGER
    TiXmlElement* triggerElement = rootHandle.FirstChild("trigger").Element();
    string triggerType("none");

    if(triggerElement) {
      do {
        TiXmlElement* triggerPositionElement;
        TiXmlElement* triggerScaleElement;
        
        Trigger trigger;
        triggerPositionElement = triggerElement->FirstChildElement("position");
        triggerScaleElement    = triggerElement->FirstChildElement("scale");

        if(triggerPositionElement) {
          triggerPositionElement->QueryFloatAttribute("x", &trigger.position.x);
          triggerPositionElement->QueryFloatAttribute("y", &trigger.position.y);
          triggerPositionElement->QueryFloatAttribute("z", &trigger.position.z);
        }
        
        if(triggerScaleElement) {
          triggerScaleElement = triggerElement->FirstChildElement("scale");
          triggerScaleElement->QueryFloatAttribute("x", &trigger.scale.x);
          triggerScaleElement->QueryFloatAttribute("y", &trigger.scale.y);
          triggerScaleElement->QueryFloatAttribute("z", &trigger.scale.z);
                      
          trigger.texture = TextureLoader::getTexture("/textures/redBox.png");
          trigger.mesh = MeshLoader::getPortalBox(trigger);
          scene->triggers.push_back(trigger);
        }
      } while((triggerElement = triggerElement->NextSiblingElement()) != NULL);
    }
    cout << "File loaded." << endl;
  } else {
    cout << "Unable to load file." << endl;
  }
  
  return scene;
}

} /* namespace glPortal */
