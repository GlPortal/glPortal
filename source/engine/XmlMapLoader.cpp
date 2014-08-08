#include "XmlMapLoader.hpp"

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

Scene* XmlMapLoader::getScene(std::string path) {
  Scene* scene = new Scene();
  TiXmlDocument doc(string(Environment::getDataDir() + path));
  bool loaded = doc.LoadFile();
 
  if(loaded) {
    TiXmlHandle docHandle(&doc);
    TiXmlElement* element;
    TiXmlHandle rootHandle(0);
    float spawnX, spawnY, spawnZ;
    float lightR(0), lightG(0), lightB(0);
    float lightX, lightY, lightZ;
    
    element = docHandle.FirstChildElement().Element();
    rootHandle=TiXmlHandle(element);
    //SPAWN
    TiXmlElement* spawnElement;
    spawnElement = rootHandle.FirstChild( "spawn" ).Element();

    if(spawnElement){
      spawnElement->QueryFloatAttribute("x", &spawnX);
      spawnElement->QueryFloatAttribute("y", &spawnY);
      spawnElement->QueryFloatAttribute("z", &spawnZ);
      scene->player.position.set(spawnX, spawnY, spawnZ);  
    } else {
      throw std::runtime_error("No spawn position defined.");
    }

    //END_SPAWN

    //LIGHT
    TiXmlElement* lightElement;
    lightElement = rootHandle.FirstChild("light").Element();

    do {
      lightElement->QueryFloatAttribute("x", &lightX);
      lightElement->QueryFloatAttribute("y", &lightY);
      lightElement->QueryFloatAttribute("z", &lightZ);

      lightElement->QueryFloatAttribute("r", &lightR);
      lightElement->QueryFloatAttribute("g", &lightG);
      lightElement->QueryFloatAttribute("b", &lightB);
      Light light;
      light.position.set(lightX, lightY, lightZ);
      light.color.set(lightR, lightG, lightB);
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

            wall.texture = TextureLoader::getTexture(Environment::getDataDir() + "/textures/" + texturePath);
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
                      
          trigger.texture = TextureLoader::getTexture(Environment::getDataDir() + "/textures/redBox.png");
          trigger.mesh = MeshLoader::getPortalBox(trigger);
          scene->triggers.push_back(trigger);
        }
        //END_TRIGGER    
      } while((triggerElement = triggerElement->NextSiblingElement()) != NULL);
    }
    cout << "File loaded." << endl;
  } else {
    cout << "Unable to load file." << endl;
  }
  
  return scene;
}
  
} /* namespace glPortal */
