#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <array>
#include <memory>
#include <list>
#include "component/Component.hpp"
#include "Entity.hpp"

namespace glPortal {

class EntityManager {
  std::list<std::unique_ptr<Entity>> entities;

public:
  
};

} /* namespace glPortal */

#endif /* ENTITY_MANAGER_HPP */
