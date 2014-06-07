#ifndef __TCP_SESSION_HPP
#define __TCP_SESSION_HPP

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include "Controller.hpp"

using boost::asio::ip::tcp;

namespace glPortal {
  class TcpSession{
  public:
    TcpSession(boost::asio::io_service& io_service);
    tcp::socket& getSocket();
    void start();
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    void handle_write(const boost::system::error_code& error);
    
  private:
    tcp::socket socket;
    enum { max_length = 1024 };
    char data[max_length];
    
    std::vector<std::string> log;
    Controller controller;
  };
}
#endif
