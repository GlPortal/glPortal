#include "Transform.hpp"
#include <engine/Entity.hpp>
#include <engine/component/Player.hpp>
#include <engine/component/RigidBody.hpp>

namespace glPortal {

void Transform::setPosition(const Vector3f &val) {
  position = val;
  if (entity.hasComponent<RigidBody>()) {
    btRigidBody &rb = *entity.getComponent<RigidBody>().body;
    btTransform t = rb.getWorldTransform();
    t.setOrigin(val);
    rb.setWorldTransform(t);
  } else if (entity.hasComponent<Player>()) {
    entity.getComponent<Player>().controller->warp(val);
  }
}

void Transform::setScale(const Vector3f &val) {
  scale = val;
}

void Transform::setOrientation(const Quaternion &val) {
  orientation = val;
  if (entity.hasComponent<RigidBody>()) {
    btRigidBody &rb = *entity.getComponent<RigidBody>().body;
    btTransform t = rb.getWorldTransform();
    t.setRotation(val);
    rb.setWorldTransform(t);
  }
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
