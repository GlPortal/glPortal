#include "Transform.hpp"
#include <engine/Entity.hpp>
#include <engine/component/RigidBody.hpp>

namespace glPortal {

void Transform::setPosition(const Vector3f &val) {
  position = val;
  if (entity.hasComponent<RigidBody>()) {
    
  }
}

void Transform::setScale(const Vector3f &val) {
  scale = val;
}

void Transform::setOrientation(const Quaternion &val) {
  orientation = val;
}

void Transform::applyModelMtx(Matrix4f &m) const {
  m.translate(position);
  m.rotate(orientation);
  m.scale(scale);
}

void Transform::getModelMtx(Matrix4f &m) const {
  m.setIdentity();
  applyModelMtx(m);
}

} /* namespace glPortal */