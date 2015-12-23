#ifndef COMPONENT_TRANSFORM_HPP
#define COMPONENT_TRANSFORM_HPP

#include "Component.hpp"
#include <engine/core/math/Vector3f.hpp>
#include <engine/core/math/Quaternion.hpp>
#include <engine/core/math/Matrix4f.hpp>

namespace glPortal {

class Transform : public Component {
protected:
  Vector3f position, scale;
  Quaternion orientation;

public:
  inline void privSetPosition(const Vector3f &v) {
    position = v;
  }
  inline void privSetScale(const Vector3f &v) {
    scale = v;
  }
  inline void privSetOrientation(const Quaternion &v) {
    orientation = v;
  }

  Transform(Entity &ent) :
    Component(ent), scale(1, 1, 1) {}

  inline const Vector3f& getPosition() const {
    return position;
  }
  void setPosition(const Vector3f&);

  inline const Vector3f& getScale() const {
    return scale;
  }
  void setScale(const Vector3f&);

  inline const Quaternion& getOrientation() const {
    return orientation;
  }
  void setOrientation(const Quaternion&);

  void applyModelMtx(Matrix4f&) const;
  void getModelMtx(Matrix4f&) const;
};

} /* namespace glPortal */

#endif /* COMPONENT_TRANSFORM_HPP */
