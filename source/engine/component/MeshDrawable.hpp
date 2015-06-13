#ifndef COMPONENT_MESHDRAWABLE_HPP
#define COMPONENT_MESHDRAWABLE_HPP

#include "Component.hpp"
#include <engine/core/math/Vector3f.hpp>
#include <assets/model/Mesh.hpp>
#include <assets/material/Material.hpp>

namespace glPortal {

class MeshDrawable : public Component {
public:
  Mesh mesh;
  // TODO: MaterialDrawable?
  Material material;

  MeshDrawable(Entity &ent) :
    Component(ent) {}
};

} /* namespace glPortal */

#endif /* COMPONENT_MESHDRAWABLE_HPP */
