#include "JavascriptSystem.hpp"

#include <assets/scene/Scene.hpp>

#include <engine/core/math/Vector3f.hpp>

#include <engine/component/Transform.hpp>
#include <engine/component/Health.hpp>

#include <engine/system/javascript/JavascriptPlayer.hpp>

namespace glPortal {

Scene* JavascriptSystem::scene;
duk_context* JavascriptSystem::ctx;
bool JavascriptSystem::savedBool = false;
std::string JavascriptSystem::script = "var player = new Player(); if(player.isGrounded()) {player.kill();}";
//std::string JavacriptSystem::script = "Kill();";

JavascriptSystem::JavascriptSystem() {
  ctx = duk_create_heap_default();

  /* FIXME: Change this */
  if (not ctx) {
    exit(1);
  }

  /* add javascript objects */
  JavascriptPlayer::init(ctx);

  /* add javascript global function 
  duk_push_c_function(ctx, Kill, 0);
  duk_put_global_string(ctx, "Kill");*/
}

JavascriptSystem::~JavascriptSystem() {
  duk_destroy_heap(ctx);
}

void JavascriptSystem::update() {
  if (duk_peval_string(ctx, script.c_str()) != 0) {
    printf("eval failed: %s\n", duk_safe_to_string(ctx, -1));
  }
}

void JavascriptSystem::setScene(Scene* n_scene) {
  scene = n_scene;
}

} /* namespace glPortal */
