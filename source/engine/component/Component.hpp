#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <cstddef>
#include <cassert>
#include <type_traits>

namespace glPortal {

using ComponentTypeId = size_t;

ComponentTypeId getNewId();
template<typename T> struct _ComponentTypeId { static ComponentTypeId id; };
template<typename T> ComponentTypeId _ComponentTypeId<T>::id { getNewId() };

class Entity;

class Component {
protected:
  Entity &entity;

public:
  using TypeId = ComponentTypeId;
  static constexpr TypeId MaxId = 32;

  template<typename T> inline static TypeId getTypeId() {
    static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
    return _ComponentTypeId<T>::id;
  }
  
  inline Component(Entity &ent) noexcept :
    entity(ent) {}

  // Forbid any copy
  Component(const Component&) = delete;
  Component& operator=(const Component&) = delete;

  virtual ~Component() {}
};

inline ComponentTypeId getNewId() {
  static ComponentTypeId lastId(0);
  return lastId++;
}

} /* namespace glPortal */

#endif /* COMPONENT_HPP */
