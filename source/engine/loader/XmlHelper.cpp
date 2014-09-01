#include "XmlHelper.hpp"

#include <tinyxml.h>
#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <vector>

#include <engine/util/Vector3f.hpp>

using namespace std;

namespace glPortal {

  std::string XmlHelper::mandatoryAttributeMessage("Mandatory attribute has not been defined.");
  /**
   * Pushes vector coordinates from an XML-element to a Vector3f
   */
  void XmlHelper::pushAttributeToVector(TiXmlElement* xmlElement, Vector3f &targetVector) {

    if(xmlElement) {
      int xQueryResult = xmlElement->QueryFloatAttribute("x", &targetVector.x);
      int yQueryResult = xmlElement->QueryFloatAttribute("y", &targetVector.y);
      int zQueryResult = xmlElement->QueryFloatAttribute("z", &targetVector.z);
      if(xQueryResult == TIXML_NO_ATTRIBUTE){
        throwMandatoryAttributeException("<x>");
      }
      if(yQueryResult == TIXML_NO_ATTRIBUTE){
        throwMandatoryAttributeException("<y>");
      }
      if(zQueryResult == TIXML_NO_ATTRIBUTE){
        throwMandatoryAttributeException("<z>");
      }
    }
  }

  void XmlHelper::throwMandatoryAttributeException(std::string message){
    throw runtime_error(mandatoryAttributeMessage + message);
  }
} /* namespace glPortal */
