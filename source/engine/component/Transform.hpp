#ifndef COMPONENT_TRANSFORM_HPP
#define COMPONENT_TRANSFORM_HPP

#include "Component.hpp"
#include <engine/core/math/Vector3f.hpp>
#include <engine/core/math/Quaternion.hpp>
#include <engine/core/math/Matrix4f.hpp>

namespace glPortal {

class Transform : public Component {
public:
  Vector3f position, scale;
  Quaternion orientation;

  Transform(Entity &ent) :
    Component(ent), scale(1, 1, 1) {}

  void applyModelMtx(Matrix4f&) const;
  void getModelMtx(Matrix4f&) const;
};

} /* namespace glPortal */

#endif /* COMPONENT_TRANSFORM_HPP */
