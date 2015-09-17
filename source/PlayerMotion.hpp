#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <array>
#include <random>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <engine/Entity.hpp>
#include <engine/component/Transform.hpp>
#include <engine/component/RigidBody.hpp>
#include <engine/core/math/Vector2f.hpp>
#include <engine/core/math/Vector3f.hpp>

namespace glPortal {

const Vector3f PLAYER_SIZE(0.5, 1, 0.5);
const float RUNNING_SPEED = 0.1f;
const float JUMP_SPEED = 0.15f;
const float HURT_VELOCITY = 0.18f;

const std::array<const std::string,2> PLAYER_PANTING_SOUND =
{
  "/audio/sfx/character/fem_panting_1.ogg",
  "/audio/sfx/character/fem_panting_2.ogg"
};

const std::array<const std::string,2> PLAYER_JUMP_SOUND =
{
  "/audio/sfx/character/fem_jump_1.ogg",
  "/audio/sfx/character/fem_jump_2.ogg"
};

const std::array<const std::string,2> PLAYER_FALL_SOUND =
{
  "/audio/sfx/character/fem_fall_1.ogg",
  "/audio/sfx/character/fem_fall_2.ogg"
};

const std::array<const std::string,6> PLAYER_FOOT_SOUND =
{
  "/audio/sfx/character/fem_foot_1.ogg",
  "/audio/sfx/character/fem_foot_2.ogg",
  "/audio/sfx/character/fem_foot_3.ogg",
#include <bullet/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>
  "/audio/sfx/character/fem_foot_4.ogg",
  "/audio/sfx/character/fem_foot_5.ogg",
  "/audio/sfx/character/fem_foot_6.ogg"
};

class PlayerMotion : public Component {
public:
  PlayerMotion(Entity &ent) :
    Component(ent),
    flying(false),
    noclip(false)/*,
    shape(.4, 1)*/ {
    entity.getComponent<Transform>().scale = PLAYER_SIZE;

    velocity.set(0, 0, 0);
    speed = RUNNING_SPEED;

    grounded = true;
    std::random_device rd;
    generator =  std::mt19937(rd());
    stepCounter = 0.0f;

    btRigidBody &rb = *entity.getComponent<RigidBody>().body;
    rb.setAngularFactor(btVector3(0, 0, 0));
    rb.setFriction(1.5);
    /*ghostObj.reset(new btPairCachingGhostObject);
    physController.reset(new btKinematicCharacterController(ghostObj.get(), &shape, 0.2));*/
  }

  // Movement
  void mouseLook();
  void move(float dtime);

  Vector3f velocity, rotation;
  bool grounded;
  bool flying, noclip;

  float speed;
private:
  /*btCapsuleShape shape;
  std::unique_ptr<btPairCachingGhostObject> ghostObj;
  std::unique_ptr<btKinematicCharacterController> physController;*/
  std::mt19937 generator;
  float stepCounter;
};

} /* namespace glPortal */

#endif /* PLAYER_HPP */
