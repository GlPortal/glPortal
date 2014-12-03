#ifndef XMLHELPER_HPP
#define XMLHELPER_HPP
#include <engine/core/math/Vector3f.hpp>
#include <engine/Entity.hpp>
#include <tinyxml.h>
#include <string>

namespace glPortal {

class XmlHelper {
public:
  static void pushAttributeVertexToVector(TiXmlElement* xmlElement, Vector3f &targetVector);
  static std::string mandatoryAttributeMessage;
  static std::string invalidElementMessage;
  static void throwMandatoryAttributeException(std::string message);
  static void extractPositionAndRotation(TiXmlElement* spawnElement, Entity &entity);
};

} /* namespace glPortal */

#endif /* XMLHELPER_HPP */
