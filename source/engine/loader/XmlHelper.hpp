#ifndef XMLHELPER_HPP
#define XMLHELPER_HPP
#include "engine/util/Vector3f.hpp"
#include <tinyxml.h>

namespace glPortal {

class XmlHelper {
public:
  static void pushAttributeToVector(TiXmlElement* xmlElement, Vector3f &targetVector);
};

} /* namespace glPortal */

#endif /* XMLHELPER_HPP */
