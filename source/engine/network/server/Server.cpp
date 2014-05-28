#include "Server.hpp"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <fstream>
#include <boost/filesystem.hpp>

using boost::asio::ip::udp;

Server::Server(boost::asio::io_service& io_service, short port)
  : io_service(io_service), socket(io_service, udp::endpoint(udp::v4(), port)){
  socket.async_receive_from(boost::asio::buffer(data, max_length), sender_endpoint,
                             boost::bind(&Server::getReceiveHandle, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Server::getReceiveHandle(const boost::system::error_code& error, size_t bytes_recvd){
  if(!error && bytes_recvd > 0){
    socket.async_send_to(boost::asio::buffer(data, bytes_recvd), sender_endpoint,
                          boost::bind(&Server::getSendHandle, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    
  } else {
    socket.async_receive_from(boost::asio::buffer(data, max_length), sender_endpoint,
                               boost::bind(&Server::getReceiveHandle, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }
}

void Server::getSendHandle(const boost::system::error_code&, size_t){
  //      for(int i=0;i<6024;i++){
  //      std::cout << data[i];
  //  }
  socket.async_receive_from(
                             boost::asio::buffer(data, max_length), sender_endpoint,
                             boost::bind(&Server::getReceiveHandle, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

