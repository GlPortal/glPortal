#include "TcpSession.hpp"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <fstream>
#include <boost/filesystem.hpp>


namespace glPortal {
  TcpSession::TcpSession(boost::asio::io_service& io_service): socket_(io_service){}

  tcp::socket& TcpSession::socket()
  {
    return socket_;
  }

  void TcpSession::start()
  {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            boost::bind(&TcpSession::handle_read, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
  }

  void TcpSession::handle_read(const boost::system::error_code& error,
                               size_t bytes_transferred)
  {
    if(!error) {
      boost::asio::async_write(socket_,
                               boost::asio::buffer(data_, bytes_transferred),
                               boost::bind(&TcpSession::handle_write, this,
                                           boost::asio::placeholders::error));
    } else {
      delete this;
    }
  }

  void TcpSession::handle_write(const boost::system::error_code& error)
  {
    if (!error){
      socket_.async_read_some(boost::asio::buffer(data_, max_length),
                              boost::bind(&TcpSession::handle_read, this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
    } else {
      delete this;
    }
  }
}
