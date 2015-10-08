#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <list>
#include "Entity.hpp"

namespace glPortal {

class Scene;

class EntityManager : public std::list<Entity> {
public:
  Scene &scene;
  EntityManager(Scene &scene);

  Entity& create();
};

} /* namespace glPortal */

#endif /* ENTITY_MANAGER_HPP */
