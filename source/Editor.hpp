#ifndef EDITOR_HPP
#define EDITOR_HPP

namespace glPortal {

class Window;
class World;

class Editor {
private:
  Window &window;
  World &world;

public:
  Editor(Window&, World&);

  void render();
};

} /* namespace glPortal */

#endif /* EDITOR_HPP */
