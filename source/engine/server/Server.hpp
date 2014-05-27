#ifndef __SERVER_HPP
#define __SERVER_HPP

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;


class Server {
public:
  Server(boost::asio::io_service& io_service, short port);
  void handle_receive_from(const boost::system::error_code& error,size_t bytes_recvd);
  void handle_send_to(const boost::system::error_code& /*error*/,  size_t /*bytes_sent*/);
private:
  boost::asio::io_service& io_service_;
  udp::socket socket_;
  udp::endpoint sender_endpoint_;
  enum { max_length = 1024 };
  char data_[max_length];
};
#endif
