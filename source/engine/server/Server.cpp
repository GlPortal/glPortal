#include "Server.hpp"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

Server::Server(boost::asio::io_service& io_service, short port)
  : io_service_(io_service),
    socket_(io_service, udp::endpoint(udp::v4(), port))
{
  socket_.async_receive_from(
                             boost::asio::buffer(data_, max_length), sender_endpoint_,
                             boost::bind(&Server::handle_receive_from, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

void Server::handle_receive_from(const boost::system::error_code& error,
                                 size_t bytes_recvd)
{
  if (!error && bytes_recvd > 0)
    {
      socket_.async_send_to(
                            boost::asio::buffer(data_, bytes_recvd), sender_endpoint_,
                            boost::bind(&Server::handle_send_to, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
    }
  else
    {
      socket_.async_receive_from(
                                 boost::asio::buffer(data_, max_length), sender_endpoint_,
                                 boost::bind(&Server::handle_receive_from, this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    }
}

void Server::handle_send_to(const boost::system::error_code& /*error*/,
                            size_t /*bytes_sent*/)
{
  socket_.async_receive_from(
                             boost::asio::buffer(data_, max_length), sender_endpoint_,
                             boost::bind(&Server::handle_receive_from, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

