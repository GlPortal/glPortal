#ifndef OBSERVER_HPP
#define OBSERVER_HPP

namespace glPortal {

class Observer {
public:
  ~Observer();
  virtual void execute(){};
};

}

#endif /* OBSERVER_HPP */
