#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

namespace glPortal {

class Viewport {
public:
  virtual void getSize(int *w, int *h) const = 0;
};

} /* namespace glPortal */

#endif /* VIEWPORT_HPP */
