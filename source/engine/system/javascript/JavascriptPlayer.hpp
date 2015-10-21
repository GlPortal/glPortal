#ifndef JAVASCRIPT_PLAYER_HPP
#define JAVASCRIPT_PLAYER_HPP

#include <duktape/duktape.h>

namespace glPortal {

class JavascriptPlayer {
public:
  static void init(duk_context *ctx);

  static duk_ret_t constructor(duk_context *ctx);

  static duk_ret_t PositionY(duk_context *ctx);

  /* Health */
  static duk_ret_t health(duk_context *ctx);
  static duk_ret_t isAlive(duk_context *ctx);
  static duk_ret_t heal(duk_context *ctx);
  static duk_ret_t harm(duk_context *ctx);
  static duk_ret_t kill(duk_context *ctx);
  static duk_ret_t revive(duk_context *ctx);

  /* States */
  static duk_ret_t isGrounded(duk_context *ctx);
  static duk_ret_t isFlying(duk_context *ctx);
};

} /* namespace glPortal */

#endif /* JAVASCRIPT_PLAYER_HPP */
