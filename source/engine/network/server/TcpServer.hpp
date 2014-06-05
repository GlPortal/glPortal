#ifndef __TCP_SERVER_HPP
#define __TCP_SERVER_HPP

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <vector>
#include <string>
#include "Controller.hpp"
#include "TcpSession.hpp"

using boost::asio::ip::tcp;

namespace glPortal {
  class TcpServer
  {
  public:
    TcpServer(boost::asio::io_service& io_service, short port);    
    void handle_accept(TcpSession* session, const boost::system::error_code& error);
  private:
    boost::asio::io_service& service;
    tcp::acceptor acceptor;

    std::vector<std::string> log;
    Controller controller;
  };
}
#endif
