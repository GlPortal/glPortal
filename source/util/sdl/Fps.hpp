#ifndef FPS_HPP
#define FPS_HPP

namespace glPortal {

class Fps {
public:
  Fps();
  int getFps();
  void countCycle();
private:
  unsigned int lastFpsTime;
  int fps;
  int skipped;
  int rendered;
};

} /* namespace glPortal */

#endif /* FPS_HPP */
