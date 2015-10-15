#ifndef JAVASCRIPT_SYSTEM_HPP
#define JAVASCRIPT_SYSTEM_HPP

#include <string>
#include <duktape/duktape.h>

namespace glPortal {

class Entity;
class Scene;

class JavascriptSystem {
public:
  JavascriptSystem();
  ~JavascriptSystem();

  void setScene(Scene* n_scene);
  void update();

  static Scene* scene;
  static duk_context *ctx;
  static bool savedBool;
  static std::string script;
};

} /* namespace glPortal */

#endif /* JAVASCRIPT_SYSTEM_HPP */
