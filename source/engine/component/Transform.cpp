#include "Transform.hpp"

namespace glPortal {

void Transform::applyModelMtx(Matrix4f &m) const {
  m.translate(position);
  m.rotate(rotation);
  m.scale(scale);
}

void Transform::getModelMtx(Matrix4f &m) const {
  m.setIdentity();
  applyModelMtx(m);
}

} /* namespace glPortal */
