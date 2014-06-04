#include "UdpServer.hpp"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <fstream>
#include <boost/filesystem.hpp>

using boost::asio::ip::udp;

namespace glPortal {     
  UdpServer::UdpServer(boost::asio::io_service& io_service, short port)
    : io_service(io_service), socket(io_service, udp::endpoint(udp::v4(), port)){
    socket.async_receive_from(boost::asio::buffer(data, max_length), sender_endpoint,
                              boost::bind(&UdpServer::getReceiveHandle, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }

  void UdpServer::getReceiveHandle(const boost::system::error_code& error, size_t bytes_recvd){
    if(!error && bytes_recvd > 0){
      std::string message(data, data + bytes_recvd);
      std::string response = controller.setMessage(message);

      
      socket.async_send_to(boost::asio::buffer(response, response.size() + bytes_recvd), sender_endpoint,
                           boost::bind(&UdpServer::getSendHandle, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

    } else {
      socket.async_receive_from(boost::asio::buffer(data, max_length), sender_endpoint,
                                boost::bind(&UdpServer::getReceiveHandle, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
  }

  void UdpServer::getSendHandle(const boost::system::error_code&, size_t){
    socket.async_receive_from(
                              boost::asio::buffer(data, max_length), sender_endpoint,
                              boost::bind(&UdpServer::getReceiveHandle, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }
}
