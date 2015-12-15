#ifndef INPUT_HPP
#define INPUT_HPP

#include <vector>
#include <string>

namespace glPortal {

class Input {
public:
  static void keyPressed(int key, int mod);
  static void keyReleased(int key, int mod);
  static bool isKeyDown(int key);
  static std::string getCharBuffer();
  static void addToBuffer(std::string character);
  static void clearBuffer();
  static void clear();
private:
  static std::vector<bool> keystates;
  static std::string charbuffer;
};

} /* namespace glPortal */

#endif /* INPUT_HPP */
