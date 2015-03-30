#ifndef INPUT_HPP
#define INPUT_HPP

namespace glPortal {

const int KEY_BUFFER = 300;

class Input {
public:
  static void keyPressed(int key, int mod);
  static void keyReleased(int key, int mod);
  static bool isKeyDown(int key);
private:
  static bool keystates[KEY_BUFFER];
};

} /* namespace glPortal */

#endif /* INPUT_HPP */
