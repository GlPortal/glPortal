#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <type_traits>
#include <memory>
#include <array>
#include <engine/component/Component.hpp>
#include <engine/env/System.hpp>

namespace glPortal {

class EntityManager;

class Entity {
public:
  EntityManager &manager;
  Entity(EntityManager &manager) :
    manager(manager) {
  }

  std::array<std::unique_ptr<Component>, Component::MaxId> components;

  template<typename T, typename... TArgs>
  T& addComponent(TArgs&&... mArgs) {
    static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
    if (hasComponent<T>()) {
      System::Log(Warning) << "Overwriting a " << typeid(T).name() << " component";
    }
    T* result(new T(*this, std::forward<TArgs>(mArgs)...));
    Component::TypeId id = Component::getTypeId<T>();
    components[id] = std::unique_ptr<Component>(result);
    return *result;
  }

  template<typename T>
  inline bool hasComponent() const {
    static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
    return components[Component::getTypeId<T>()].get() != nullptr;
  }

  template<typename T>
  inline T& getComponent() const {
    static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
    return *reinterpret_cast<T*>(components[Component::getTypeId<T>()].get());
  }

  void clearComponents() {
    for (std::unique_ptr<Component> &p : components) {
      p.reset(nullptr);
    }
  }
};

} /* namespace glPortal */

#endif /* ENTITY_HPP */
