#ifndef EDITOR_HPP
#define EDITOR_HPP

namespace glPortal {

class World;

class Editor {
private:
  World &world;

public:
  Editor(World&);

  void renderUI();
};

} /* namespace glPortal */

#endif /* EDITOR_HPP */
