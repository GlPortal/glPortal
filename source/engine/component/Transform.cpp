#include "Transform.hpp"

namespace glPortal {

void Transform::getModelMtx(Matrix4f &m) const {
  m.setIdentity();
  m.translate(position);
  m.rotate(rotation);
  m.scale(scale);
}

} /* namespace glPortal */