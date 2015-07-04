#ifndef XMLHELPER_HPP
#define XMLHELPER_HPP

#include <engine/core/math/Vector3f.hpp>
#include <engine/Entity.hpp>
#include <tinyxml2.h>
#include <string>

using namespace tinyxml2;

namespace glPortal {

class XmlHelper {
public:
  static void pushAttributeVertexToVector(XMLElement *xmlElement, Vector3f &targetVector);
  static std::string mandatoryAttributeMessage;
  static std::string invalidElementMessage;
  static void throwMandatoryAttributeException(const std::string &message);
  static void extractPosition(XMLElement *xmlElement, Vector3f &position);
  static void extractRotation(XMLElement *xmlElement, Vector3f &rotation);
  static void extractScale(XMLElement *xmlElement, Vector3f &scale);
};

} /* namespace glPortal */

#endif /* XMLHELPER_HPP */
