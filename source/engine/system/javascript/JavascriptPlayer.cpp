#include "JavascriptPlayer.hpp"

#include <assets/scene/Scene.hpp>

#include <engine/core/math/Vector3f.hpp>

#include <engine/component/Transform.hpp>
#include <engine/component/Health.hpp>

#include <engine/system/JavascriptSystem.hpp>

#include <PlayerMotion.hpp>

namespace glPortal {

void JavascriptPlayer::init(duk_context* ctx) {
  const duk_function_list_entry PlayerMethods[] = {
    /* Health */
    {"health", health, 0},
    {"isAlive", isAlive, 0},
    {"heal", heal, 1},
    {"harm", harm, 1},
    {"kill", kill, 0},
    {"revive", revive, 0},

    /* States */
    {"isGrounded", isGrounded, 0},
    {"isFlying", isFlying, 0},

    {nullptr, nullptr, 0}
  };

  duk_push_c_function(ctx, constructor, 1);
  duk_push_object(ctx);
  duk_put_function_list(ctx, -1, PlayerMethods);
  duk_put_prop_string(ctx, -2, "prototype");

  duk_put_global_string(ctx, "Player");
}

duk_ret_t JavascriptPlayer::constructor(duk_context* ctx) {
  if (not duk_is_constructor_call(ctx)) {
    return DUK_RET_TYPE_ERROR;
  }
  duk_push_this(ctx);

  return 0;
}

duk_ret_t JavascriptPlayer::PositionY(duk_context* ctx) {
  duk_push_this(ctx);
  
  Entity &player = JavascriptSystem::scene->player;
  float y = player.getComponent<Transform>().position.y;

  duk_push_number(ctx, y);

  return 1;
}

/* Health */
duk_ret_t JavascriptPlayer::health(duk_context* ctx) {
  duk_push_this(ctx);

  Entity &player = JavascriptSystem::scene->player;
  float health = player.getComponent<Health>().health;

  duk_push_number(ctx, health);

  return 1;
}

duk_ret_t JavascriptPlayer::isAlive(duk_context* ctx) {
  duk_push_this(ctx);

  Entity &player = JavascriptSystem::scene->player;
  bool isAlive = player.getComponent<Health>().isAlive();

  duk_push_boolean(ctx, isAlive);

  return 1;
}

duk_ret_t JavascriptPlayer::heal(duk_context* ctx) {
  duk_push_this(ctx);

  /* maybe add (float) */
  float heal = duk_require_number(ctx, -3);

  Entity &player = JavascriptSystem::scene->player;
  player.getComponent<Health>().heal(heal);

  return 0;
}

duk_ret_t JavascriptPlayer::harm(duk_context* ctx) {
  duk_push_this(ctx);

  float harm = duk_require_number(ctx, -3);

  Entity &player = JavascriptSystem::scene->player;
  player.getComponent<Health>().harm(harm);

  return 0;
}

duk_ret_t JavascriptPlayer::kill(duk_context* ctx) {
  duk_push_this(ctx);

  Entity &player = JavascriptSystem::scene->player;
  player.getComponent<Health>().kill();

  return 0;
}

duk_ret_t JavascriptPlayer::revive(duk_context* ctx) {
  duk_push_this(ctx);

  Entity &player = JavascriptSystem::scene->player;
  player.getComponent<Health>().revive();

  return 0;
}
/* Health */

/* States */
duk_ret_t JavascriptPlayer::isGrounded(duk_context* ctx) {
  duk_push_this(ctx);

  Entity &player = JavascriptSystem::scene->player;
  bool grounded = player.getComponent<PlayerMotion>().grounded;

  duk_push_boolean(ctx, grounded);

  return 1;
}

duk_ret_t JavascriptPlayer::isFlying(duk_context* ctx) {
  duk_push_this(ctx);

  Entity &player = JavascriptSystem::scene->player;
  bool flying = player.getComponent<PlayerMotion>().flying;

  duk_push_boolean(ctx, flying);

  return 1;
}
/* States */
} /* namespace glPortal */
