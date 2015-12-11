#ifndef INPUT_HPP
#define INPUT_HPP

#include <vector>

namespace glPortal {

class Input {
public:
  static void keyPressed(int key, int mod);
  static void keyReleased(int key, int mod);
  static bool isKeyDown(int key);
  static void clear();
private:
  static std::vector<bool> keystates;
};

} /* namespace glPortal */

#endif /* INPUT_HPP */
