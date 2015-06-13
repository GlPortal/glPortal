#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <type_traits>
#include <memory>
#include <bitset>

#include <engine/component/Component.hpp>
#include <engine/env/System.hpp>

namespace glPortal {

class Entity {
public:
  Entity() {}

  std::array<std::unique_ptr<Component>, Component::MaxCount> components;
  std::bitset<Component::MaxCountBits> componentBits;

  template<typename T, typename... TArgs>
  T& addComponent(TArgs&&... mArgs) {
    static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
    if (hasComponent<T>()) {
      System::log("");
    }
    T* result(new T(*this, std::forward<TArgs>(mArgs)...));
    Component::TypeId id = Component::getTypeId<T>();
    components[id] = std::unique_ptr<Component>(result);
    componentBits[id] = true;
    return *result;
  }

  template<typename T>
  inline bool hasComponent() const {
    static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
    return componentBits[Component::getTypeId<T>()];
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
    componentBits.reset();
  }
};

} /* namespace glPortal */

#endif /* ENTITY_HPP */
