#ifndef TRIGGER_HPP
#define TRIGGER_HPP

#include <string>

#include <engine/TechVolEntity.hpp>

#include <engine/core/math/Vector3f.hpp>

namespace glPortal {

class Trigger: public TechVolEntity {
public:
  Trigger();

  std::string type;
};

} /* namespace glPortal */

#endif /* TRIGGER_HPP */
