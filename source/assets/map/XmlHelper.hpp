#ifndef XMLHELPER_HPP
#define XMLHELPER_HPP
#include <engine/core/math/Vector3f.hpp>
#include <engine/Entity.hpp>
#include <tinyxml.h>
#include <string>

namespace glPortal {

class XmlHelper {
public:
  static void pushAttributeVertexToVector(TiXmlElement *xmlElement, Vector3f &targetVector);
  static std::string mandatoryAttributeMessage;
  static std::string invalidElementMessage;
  static void throwMandatoryAttributeException(const std::string &message);
  static void extractPosition(TiXmlElement *xmlElement, Vector3f &position);
  static void extractRotation(TiXmlElement *xmlElement, Vector3f &rotation);
  static void extractScale(TiXmlElement *xmlElement, Vector3f &scale);
};

} /* namespace glPortal */

#endif /* XMLHELPER_HPP */
