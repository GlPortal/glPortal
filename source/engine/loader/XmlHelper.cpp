#include "XmlHelper.hpp"

#include <tinyxml.h>
#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>

#include "engine/util/Vector3f.hpp"

using namespace std;

namespace glPortal {

/*
 * Pushes vector coordinates from an element to a Vector3f
 */
  void XmlHelper::pushAttributeToVector(TiXmlElement* xmlElement, Vector3f &targetVector) {

    if(xmlElement) {
      xmlElement->QueryFloatAttribute("x", &targetVector.x);
      xmlElement->QueryFloatAttribute("y", &targetVector.y);
      xmlElement->QueryFloatAttribute("z", &targetVector.z);
    }
}  
} /* namespace glPortal */
