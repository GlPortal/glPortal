#include "Client.hpp"
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "../../Environment.hpp"

using boost::asio::ip::udp;

namespace glPortal{
  std::string Client::sendMessage(std::string message){
    ConfigFileParser * config;
    config = glPortal::engine::Environment::getConfigPointer();
    std::string server, port, username;
    try{
      server  = config->getFloatByKey("server");
      port    = config->getFloatByKey("port");
    } catch (const std::invalid_argument& e){
      server  = "glportal.de";
      port    = "101010";
    }
    
    int max_length = 1024;
    boost::asio::io_service io_service;
    
    udp::socket socket(io_service, udp::endpoint(udp::v4(), 0));
    
    udp::resolver resolver(io_service);
    udp::resolver::query query(udp::v4(), server, port);
    udp::resolver::iterator iterator = resolver.resolve(query);
    socket.send_to(boost::asio::buffer(message, message.size()), *iterator);

    char reply[max_length];
    udp::endpoint sender_endpoint;
    
    size_t reply_length = socket.receive_from(
        boost::asio::buffer(reply, max_length), sender_endpoint);

    return std::string(reply);
  }
}
