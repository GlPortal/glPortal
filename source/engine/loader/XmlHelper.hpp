#ifndef XMLHELPER_HPP
#define XMLHELPER_HPP
#include "engine/util/Vector3f.hpp"
#include <tinyxml.h>
#include <string>

namespace glPortal {

class XmlHelper {
public:
  static void pushAttributeToVector(TiXmlElement* xmlElement, Vector3f &targetVector);
  static std::string mandatoryAttributeMessage;
  static void throwMandatoryAttributeException(std::string message);
};

} /* namespace glPortal */

#endif /* XMLHELPER_HPP */
