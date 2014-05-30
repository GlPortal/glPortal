#ifndef __SERVER_HPP
#define __SERVER_HPP

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <vector>
#include <string>

using boost::asio::ip::udp;

namespace glPortal {
  namespace network {
    namespace server {
      class Server {
      public:
        Server(boost::asio::io_service& io_service, short port);
        void getReceiveHandle(const boost::system::error_code& error,size_t bytes_recvd);
        void getSendHandle(const boost::system::error_code& /*error*/,  size_t /*bytes_sent*/);
      private:
        boost::asio::io_service& io_service;
        udp::socket socket;
        udp::endpoint sender_endpoint;
        enum { max_length = 1024 };
        char data[max_length];
        std::vector<std::string> log;
      };
    }
  }
}
#endif
