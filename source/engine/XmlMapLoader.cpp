#include "XmlMapLoader.hpp"

#include <GL/glew.h>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>

#include "../Player.hpp"
#include "../Scene.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"
#include "util/Vector2f.hpp"
#include "util/Vector3f.hpp"
#include "environment/Environment.hpp"
#include <tinyxml.h>


using namespace std;

namespace glPortal {

Scene* XmlMapLoader::getScene(std::string path) {
  //  XMLDocument doc;
  //  doc.LoadFile(Environment::getDataDir() + "first.xml" );
  //TiXmlDocument doc(Environment::getDataDir() + "first.xml" );
  //  TiXmlDocument doc(std::string("/home/henry/system/projects/c++/glPortal/data/maps/first.xml"));
  TiXmlDocument doc(std::string(Environment::getDataDir() + "/maps/first.xml"));
  bool loaded = doc.LoadFile();
 
  if (loaded){
    TiXmlHandle docHandle(&doc);
    TiXmlElement* mapElement;
    TiXmlElement* element;
    TiXmlElement* spawnElement;
    TiXmlElement* spawnVectorElement;
    TiXmlHandle rootHandle(0);
    float x, y, z;
    element = docHandle.FirstChildElement().Element();
    rootHandle=TiXmlHandle(element);
    spawnVectorElement = rootHandle.FirstChild( "spawn" ).FirstChild().Element();

    if(spawnVectorElement){
      spawnVectorElement->QueryFloatAttribute("x", &x);
      spawnVectorElement->QueryFloatAttribute("y", &y);
      spawnVectorElement->QueryFloatAttribute("z", &z);
    }
    std::cout << x << " / ";
    std::cout << y << " / ";
    std::cout << z << " / ";

    std::cout << "File loaded.";
  } else {
    std::cout << "Unable to load File.";
  }
  
  Scene* scene = new Scene();
  return scene;
}

} /* namespace glPortal */
