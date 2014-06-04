#ifndef __UDP_CLIENT_HPP
#define __UDP_CLIENT_HPP

#include <string>

namespace glPortal {
  class UdpClient {
  public:
    static std::string sendMessage(std::string message);
  private:
  };
}
#endif
